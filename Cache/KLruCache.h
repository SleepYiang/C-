#pragma once
#include <cstring>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include<cmath>
#include<thread>
#include<vector>
#include "KICachePolicy.h"
namespace KamaCache
{
    //提前声明
    template <typename Key, typename Value> class KLruCache;
    template <typename Key, typename Value>
    class LruNode
    {
    public:
        LruNode(Key key, Value value)
            :Key(key)
            , Value(value)
            ,accessCount_(1){}
        //提供相关的接口
        Key getKey() const{return key_;}
        Value getValue() const{return value_;}
        void setValue(const Value &value){value_ = value;}
        size_t getAccessCount() const{return accessCount_;}
        void incrementAccessCount(){++accessCount_;}
        friend class KLruCache<Key, Value>;

    private:
        Key key_;
        Value value_;
        size_t accessCount_;        //访问次数
        std::weak_ptr<LruNode<Key, Value>> prev_;           //weak_ptr打破循环引用
        std::shared_ptr<LruNode<Key, Value>> next_;
    };

    template <typename Key, typename Value>
    class KLruCache:public KICachePolicy<Key,Value>
    {
    public:
        using LruNodeType = LruNode<Key,Value>;
        using NodePtr = std::shared_ptr<LruNodeType>;
        using NodeMap=std::unordered_map<Key,NodePtr>;

        KLruCache(int capacity):capacity_(capacity)
        {
            initializerList();
        }

        ~KLruCache() override=default;

        //添加缓存
        void put(Key key, Value value) override
        {
            //判断容量
            if (capacity_ <= 0)
                return;
            std::lock_guard<std::mutex> lock(mutex_);
            auto it=nodemap_.find(key);
            if (it != nodemap_.end())
            {
                //更新value调用get方法
                updateExistingNode(it->second,value);
                return;
            }
            addNewNode(key, value);
        }

        bool get(Key key, Value &value) override
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it=nodemap_.find(key);
            if (it != nodemap_.end())
            {
                moveToMostRecent(it->second);
                value = it->second->getValue();
                return true;
            }
            return false;
        }

        Value get(Key key) override
        {
            Value value{};
            get(key, value);
            return value;
        }

        //删除指定元素
        void remove (Key key)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it=nodemap_.find(key);
            if (it != nodemap_.end())
            {
                removeNode(it->second);
                nodemap_.erase(it);
            }
        }


    private:
        void initializerList()
        {
            //创建首位虚拟节点
            dummyHead_=std::make_shared<LruNodeType>(Key(), Value());
            dummyTail_=std::make_shared<LruNodeType>(Key(), Value());
            dummyHead_->next_=dummyTail_;
            dummyTail_->prev_=dummyHead_;
        }

        void updateExistingNode(NodePtr node,const Value &value)
        {
            node->setValue(value);
            moveToMostRecent(node);
        }

        void addNewNode(const Key &key, Value &value)
        {
            if (nodemap_.size() >= capacity_)
            {
                evictLeastRecent();
            }
            NodePtr newNode = std::make_shared<LruNodeType>(key, value);
            insertNode(newNode);
            nodemap_[key]=newNode;
        }

        //将该节点移动到新位置
        void moveToMostRecent(NodePtr node)
        {
            removeNode(node);
            insertNode(node);
        }

        void removeNode(NodePtr node)
        {
            if (!node->prev_.expired()&&!node->next_)
            {
                auto prev=node->prev_.lock();
                prev->next_=node->next_;
                node->next_->prev_=node->prev_;
                node->next_=nullptr;
            }
        }

        //从尾部插入节点
        void insertNode(NodePtr node)
        {
            node->next_=dummyTail_;
            node->prev_=dummyTail_->prev_;
            dummyTail_->prev_.lock()->next_=node;
            dummyTail_->prev_=node;
        }

        void evictLeastRecent()
        {
            NodePtr leastRecent=dummyHead_->next_;
            removeNode(leastRecent);
            nodemap_.erase(dummyHead_->getKey());
        }

        int capacity_;                  //缓存容量
        NodeMap nodemap_;
        std::mutex mutex_;
        NodePtr dummyHead_;             //虚拟头节点
        NodePtr dummyTail_;
    };



    //LUR-K版本对LUR基础版本做一次维护升级
    template <typename Key, typename Value>
    class KLruKCache:public KLruCache<Key,Value>
    {
    public:
        KLruKCache(int capacity,int historyCapacity,int k)
            :KLruCache<Key,Value>(capacity)
            ,historyList_(std::make_unique<KLruCache<Key,size_t>>(historyCapacity))
            ,k_(k){}

        Value get(Key key)
        {
            //从著缓存区获取数据
            Value value{};
            bool inMainCache=KLruCache<Key,Value>::get(key,value);

            //获取更新访问历史数据
            size_t historyCount=historyList_->get(key);
            historyCount+=1;
            historyList_->put(key, historyCount);

            //如果函数在主缓存区，直接返回
            if (inMainCache)
            {
                return value;
            }

            //如果数据不在著缓冲，但是访问次数达到了k次
            if(historyCount>=k_)
            {
                auto it=historyValueMap_.find(key);
                if (it != historyValueMap_.end())
                {
                    Value storedValue=it->second;

                    historyList_->remove(key);
                    historyValueMap_.erase(it);

                    KLruCache<Key,Value>::put(key, storedValue);
                    return storedValue;
                }
            }
            return value;
        }
        void put(Key key,Value value)
        {
            Value expiredValue{};
            bool inMainCache=KLruCache<Key,Value>::get(key,expiredValue);

            if (inMainCache)
            {
                KLruCache<Key,Value>::put(key, value);
                return ;
            }
            size_t historyCount=historyList_->get(key);
            historyCount+=1;
            historyList_->put(key, historyCount);

            historyValueMap_[key]=value;

            if (historyCount>=k_)
            {
                historyList_->remove(key);
                historyValueMap_.erase(key);
                KLruCache<Key,Value>::put(key, value);
            }
        }
    private:
        int  k_;                                                            //进入缓存的评判标准
        std::unique_ptr<KLruCache<Key,size_t>> historyList_;                //访问数据记录（value为访问次数）
        std::unordered_map<Key,Value> historyValueMap_;                     //存储访问未达到k次访问数据的值
    };

    // lru优化：对lru进行分片，提高高并发使用的性能
    template<typename Key, typename Value>
    class KHashLruCaches
    {
    public:
        KHashLruCaches(size_t capacity, int sliceNum)
            : capacity_(capacity)
            , sliceNum_(sliceNum > 0 ? sliceNum : std::thread::hardware_concurrency())//设置一个默认的分片数目保证分片数大于0
        {
            size_t sliceSize = std::ceil(capacity / static_cast<double>(sliceNum_)); // 获取每个分片的大小
            for (int i = 0; i < sliceNum_; ++i)
            {
                lruSliceCaches_.emplace_back(new KLruCache<Key, Value>(sliceSize));
            }
        }

        void put(Key key, Value value)
        {
            // 获取key的hash值，并计算出对应的分片索引
            size_t sliceIndex = Hash(key) % sliceNum_;
            lruSliceCaches_[sliceIndex]->put(key, value);
        }

        bool get(Key key, Value& value)
        {
            // 获取key的hash值，并计算出对应的分片索引
            size_t sliceIndex = Hash(key) % sliceNum_;
            return lruSliceCaches_[sliceIndex]->get(key, value);
        }

        Value get(Key key)
        {
            Value value;
            memset(&value, 0, sizeof(value));
            get(key, value);
            return value;
        }

    private:
        // 将key转换为对应hash值
        size_t Hash(Key key)
        {
            std::hash<Key> hashFunc;
            return hashFunc(key);
        }

    private:
        size_t                                              capacity_;  // 总容量
        int                                                 sliceNum_;  // 切片数量
        std::vector<std::unique_ptr<KLruCache<Key, Value>>> lruSliceCaches_; // 切片LRU缓存
    };
};
