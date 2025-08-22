#include <cstdlib>
#include<map>
#include<new>
#include<cstdio>
std::map<void*, size_t> allocated;
bool enable = false;

void* operator new(size_t size)
{
    bool was_enable = enable;
    enable = false;
    void* ptr = malloc(size);
    if (was_enable)
    {
        if (ptr == nullptr)
            throw std::bad_alloc();
        printf("new :ptr=%p,size=%zd\n", ptr, size);
        allocated[ptr] = size;
    }

    enable = true;
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    printf("delete :ptr=%p\n", ptr);
    allocated.erase(ptr);
    free(ptr);
}

int main()
{
    enable = true;
    int* p1 = new int;
    short* p2 = new short;
    delete p1;
    delete p2;
    enable = false;
    if (allocated.size() != 0)
        printf("有内存泄漏");
}