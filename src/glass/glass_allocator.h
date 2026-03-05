#pragma once

#include <list>

struct StackAllocator // sizeof=0xC
{                                       // XREF: .data:StackAllocator GlassesClient::allocator/r
    unsigned __int8 *memory;            // XREF: GlassesClient::InitAllocator(Glasses *)+1B/w
    unsigned int size;                  // XREF: GlassesClient::InitAllocator(Glasses *)+24/w
                                        // GlassesClient::GetFreeMem(void)+3/r
    unsigned int pos;                   // XREF: GlassesClient::InitAllocator(Glasses *)+2A/w
};

struct SmallAllocator // sizeof=0x18
{                                       // XREF: GlassRenderer/r
    void *memory;
    void *freeHead;
    unsigned int blockSize;
    unsigned int numBlocks;
    unsigned int numUsed;
    unsigned int maxUsed;

    void __thiscall Init(void *buffer, unsigned int bs, unsigned int nb);
    void **__thiscall Allocate(unsigned int size);
    void __thiscall Free(void **ptr, unsigned int num);
    bool __thiscall IsValidPointer(void *ptr);
};

template <typename T>
struct SmallAllocatorTemplate//<GlassPhysics * *> // sizeof=0x4
{                                       // XREF: ??0?$_List_ptr@PAUShardGroup@@V?$SmallAllocatorTemplate@PAPAUShardGroup@@@@@std@@IAE@V?$SmallAllocatorTemplate@PAPAUShardGroup@@@@@Z/r
    //SmallAllocator *alloc;              // XREF: std::list<GlassShard *,SmallAllocatorTemplate<GlassShard * *>>::list<GlassShard *,SmallAllocatorTemplate<GlassShard * *>>(SmallAllocatorTemplate<GlassShard *> const &)+14/w

    using value_type = T;

    SmallAllocator *alloc;

    SmallAllocatorTemplate() noexcept : alloc(nullptr) {}
    SmallAllocatorTemplate(SmallAllocator *a) noexcept : alloc(a) {}

    template<class U>
    SmallAllocatorTemplate(const SmallAllocatorTemplate<U> &other) noexcept
        : alloc(other.alloc) {
    }

    T *allocate(std::size_t n)
    {
        return static_cast<T *>(*alloc->Allocate(sizeof(T) * n));
    }

    void deallocate(T *p, std::size_t)
    {
        alloc->Free((void **)p, 1);
    }

    template<class U>
    struct rebind
    {
        using other = SmallAllocatorTemplate<U>;
    };
};

// KISAKTODO: this is trash and probably needs a rewrite so T* is the 1st <>, and then rewrite the instances to remove the *
template <typename T>
struct FixedSizeAllocator//<GlassPhysics *> // sizeof=0x40
{
    void *memory;
    std::list<T *> used;
    std::list<T *> free;

    unsigned int maxUsed;

    static void *operator new(size_t size);
    static void operator delete(void *ptr);

    void Init(void *mem, unsigned int numObjects)
    {
        memory = mem;

        used.clear();
        free.clear();
        maxUsed = 0;

        T *array = static_cast<T *>(mem);

        for (unsigned int i = 0; i < numObjects; ++i)
        {
            free.push_back(&array[i]);
        }
    }

    T *Allocate()
    {
        if (free.empty())
            __debugbreak(); // out-of-memory

        T *obj = free.front();
        free.pop_front();

        used.push_back(obj);

        if (used.size() > maxUsed)
            maxUsed = (unsigned int)used.size();

        return obj;
    }

    void Free(T *obj)
    {
        used.remove(obj);
        free.push_back(obj);
    }

    void FreeAll()
    {
        while (!used.empty())
        {
            T *obj = used.front();
            used.pop_front();
            free.push_back(obj);
        }
    }
};

struct Allocator // sizeof=0x14
{                                       // XREF: GlassRenderer/r
    struct Memory // sizeof=0x14
    {
        Allocator::Memory *next;
        Allocator::Memory *prev;
        void *userData;
        Allocator::Memory *nextFree;
        Allocator::Memory *prevFree;

        void __thiscall Init();
        char __thiscall MakeFree();
    };
    void *buffer;
    Allocator::Memory *head;
    Allocator::Memory *tail;
    Allocator::Memory *freeHead;
    int maxUsed;


    void __thiscall Init(void *buf, int size);
    void __thiscall FreeAll();
    Memory **__thiscall Allocate(int size, void *userData);
    void __thiscall Free(unsigned int *ptr);
    unsigned int __thiscall GetMemorySize(unsigned int *ptr);
    void __thiscall Split(Memory *mem, int size);
    void __thiscall Merge(Memory *mem1, Memory *mem2);
    int __thiscall Defrag(void(__cdecl *func)(void *), int count);
    int __thiscall GetFree();
    int __thiscall GetLargestFree();
};
