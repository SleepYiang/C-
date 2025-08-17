#pragma once
#include <algorithm>
#include<iostream>
#include<thread>
#include<vector>
#include<memory>
#include<mutex>
#include<shared_mutex>
#include<iterator>
#include<map>
#include<list>
template<typename Key,typename Value,typename Hash=std::hash<Key>>
class threadsafe_lookup_table
{
private:
    class bucket_type
    {
        friend class threadsafe_lookup_table;
    private:
        //存储元素的类型为pair，
        typedef std::pair<Key,Value> bucket_value;
        //链表存储的元素
        typedef std::list<bucket_value> bucket_data;
        //链表的迭代器
        typedef typename bucket_data::iterator bucket_iterator;
        //链表数据
        bucket_data data;
        //改用共享锁
        mutable std::shared_mutex mutex;

        bucket_iterator find_entry_for(const Key&key)
        {
            return std::find_if(data.begin(),data.end(),[&](bucket_value const& item)
            {
                return item.first==key;
            });
        }
    public:
        //查找key,找到则返回对应的value,没找到返回默认值
        Value value_for(Key const&key,Value const&default_value)
        {
            std::shared_lock<std::shared_mutex>lock(mutex);
            auto  found_entry=find_entry_for(key);
            return(found_entry==data.end())?default_value:found_entry->second;
        }
        //添加key和value,找到则更新，没找到则添加
        void add_or_update_mapping(Key const&key,Value const&value)
        {
            std::unique_lock<std::shared_mutex>lock(mutex);
            auto found_entry=find_entry_for(key);
            if (found_entry==data.end())
            {
                data.push_back(bucket_value(key,value));
            }else
            {
                found_entry->second=value;
            }
        }
        //删除对应的key
        void remove_mapping(Key const&key)
        {
            std::unique_lock<std::shared_mutex>lock(mutex);
            auto found_entry=find_entry_for(key);
            if (found_entry!=data.end())
            {
                data.erase(found_entry);
            }
        }
    };
private:
    //用vector存储痛的类型
    std::vector<std::unique_ptr<bucket_type>> buckets;
    //hash<Key>哈希表用来根据key生成哈希表
    Hash hasher;
    bucket_type&get_bucket(Key const&key) const
    {
        std::size_t const bucket_index=hasher(key)%buckets.size();
        return *(buckets[bucket_index]);
    }
public:
    threadsafe_lookup_table(unsigned num_buckets=19,Hash const &hasher_=Hash()):buckets(num_buckets),hasher(hasher_)
    {
        for (unsigned i=0;i<num_buckets;++i)
        {
            buckets[i].reset(new bucket_type);
        }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const&other)=delete;
    threadsafe_lookup_table&operator=(threadsafe_lookup_table const&other)=delete;

    Value value_for(Key const&key,Value const&default_value=Value())
    {
        return get_bucket(key),value_for(key,default_value);
    }
    void add_or_update_mapping(Key const&key,Value const&value)
    {
        return get_bucket(key),add_or_update_mapping(key,value);
    }
    void remove_mapping(Key const&key)
    {
        return get_bucket(key),remove_mapping(key);
    }
    std::map<Key,Value> get_map()
    {
        std::vector<std::unique_lock<std::shared_mutex>>locks;
        for (unsigned i=0;i<buckets.size();++i)
        {
            locks.push_back(std::unique_lock<std::shared_mutex>(buckets[i]->mutex));
        }
        std::map<Key,Value>res;
        for (unsigned i=0;i<locks.size();++i)
        {
            //简化版本
            auto it=buckets[i]->data.begin();
            //详细版本
            //typename bucket_type::bucket_iterator_it=buckets[i]->data.begin();
            for (;it!=buckets[i]->data.end();++it)
            {
                res.insert(*it);
            }
        }
        return res;
    }
};