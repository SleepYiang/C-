#include "MemoryPool_V1_0.h"

namespace MemoryPoolV1_0
{
    MemoryPoolV1_0::MemoryPoolV1_0(size_t BlockSize): BlockSize_(BlockSize){}

    MemoryPoolV1_0::~MemoryPoolV1_0()
    {
        Slot* cur = firstBlock_;
        while (cur)
        {
            Slot* next = cur->next;
            operator delete(reinterpret_cast<void*>(cur));
            cur = next;
        }
    }

    void MemoryPoolV1_0::init(size_t size)
    {
        assert(size>0);
        SlotSize_ = size;
        firstBlock_ = nullptr;
        freeList_ = nullptr;
        curSlot_ = nullptr;
        lastSlot_ = nullptr;
    }

    void* MemoryPoolV1_0::allocate()
    {
        if (freeList_ != nullptr)
        {
            {
                std::lock_guard<std::mutex> lock(mutexForFreeList_);
                if (freeList_ != nullptr)
                {
                    Slot* temp = freeList_;
                    freeList_ = freeList_->next;
                    return temp;
                }
            }
        }
        Slot* temp;
        {
            std::lock_guard<std::mutex>lock(mutexForBlock_);
            if (curSlot_ == lastSlot_)
            {
                allocateNewBlock();
            }
            temp = curSlot_;
            curSlot_+=SlotSize_/sizeof(Slot);
        }
        return temp;
    }
    void MemoryPoolV1_0::deallocate(void*ptr)
    {
        if (ptr)
        {
            std::lock_guard<std::mutex> lock(mutexForFreeList_);
            reinterpret_cast<Slot*>(ptr)->next = freeList_;
            freeList_ = reinterpret_cast<Slot*>(ptr);
        }
    }
    void MemoryPoolV1_0::allocateNewBlock()
    {
        void*newBlock=operator new(SlotSize_);
        reinterpret_cast<Slot*>(newBlock)->next = freeList_;
        freeList_ = reinterpret_cast<Slot*>(newBlock);

        char*boby=reinterpret_cast<char*>(newBlock)+sizeof(Slot*);
        size_t paddingSize=padPointer(boby,SlotSize_);
        curSlot_=reinterpret_cast<Slot*>(boby+paddingSize);
        lastSlot_=reinterpret_cast<Slot*>(reinterpret_cast<size_t>(newBlock)+SlotSize_);
        freeList_=nullptr;
    }
    size_t MemoryPoolV1_0::padPointer(char*p,size_t align)
    {
        return (align-reinterpret_cast<size_t>(p)%align);
    }
    void HashBucket::initMemoryPoolV1_0()
    {
        for (int i = 0; i < MEMORY_POOL_NUM; i++)
        {
            getMemoryPoolV1_0(i).init((i+1)*SLOT_BASE_SIZE);
        }
    }
    MemoryPoolV1_0&HashBucket::getMemoryPoolV1_0(int index)
    {
        static MemoryPoolV1_0 memoryPool[MEMORY_POOL_NUM];
        return memoryPool[index];
    }
}
