#ifndef MEMORYPOOL_V1_0_H
#define MEMORYPOOL_V1_0_H
#include<mutex>
#include<cassert>
#include
namespace MemoryPoolV1_0
{
#define MEMORY_POOL_NUM 64
#define SLOT_BASE_SIZE 8
#define MAX_SLOT_SIZE 512

    struct Slot
    {
        Slot* next;
    };

    class MemoryPoolV1_0
    {
    public:
        MemoryPoolV1_0(size_t BlockSize = 4096);
        ~MemoryPoolV1_0();

        void init(size_t);
        void* allocate();
        void deallocate(void*);

    private:
        void allocateNewBlock();
        size_t padPointer(char* p, size_t align);

    private:
        int BlockSize_;
        int SlotSize_;
        Slot* firstBlock_;
        Slot* curSlot_;
        Slot* freeList_;
        Slot* lastSlot_;
        std::mutex mutexForFreeList_;
        std::mutex mutexForBlock_;
    };

    class HashBucket
    {
    public:
        static void initMemoryPoolV1_0();
        static MemoryPoolV1_0& getMemoryPoolV1_0(int index);

        static void* useMemory(size_t size)
        {
            if (size <= 0)
                return nullptr;
            if (size > MAX_SLOT_SIZE)
                return operator new(size);
            return getMemoryPoolV1_0(((size + 7) / SLOT_BASE_SIZE) - 1).allocate();
        }

        static void freeMemory(void* ptr, size_t size)
        {
            if (!ptr)
                return;
            if (size > MAX_SLOT_SIZE)
            {
                operator delete(ptr);
                return;
            }
            getMemoryPoolV1_0(((size + 7) / SLOT_BASE_SIZE) - 1).deallocate(ptr);
        }

        template <typename T, typename... Args>
        friend T* newElement(Args&&... args);
    };
    template <typename T, typename... Args>
    T* newElemnent(Args&&... args)
    {
        T*p=nullptr;
        if ((p=reinterpret_cast<T*>(HashBucket::useMemory(sizeof(T))))!=nullptr)
            new(p) T(std::forward<Args>(args)...);
        return p;
    }
    template<typename T>
    void deleteElement(T*p)
    {
        if (p)
        {
            p->~T();
            HashBucket::freeMemory(reinterpret_cast<void*>(p),sizeof(T));
        }
    }
}
#endif //MEMORYPOOL_V1_0_H
