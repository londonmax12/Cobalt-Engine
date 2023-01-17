#pragma once
#include "Logging/Logging.h"
#include "Memory.h"

namespace Ikigai {
    template <typename T>
    class PoolAllocator {
    public:
        PoolAllocator(size_t size);
        ~PoolAllocator();

        T* Allocate();
        void Deallocate(T* p);
    private:
        char* m_Buffer;
        void** m_FreeList;
        size_t m_Size;
        size_t m_NextFree = m_Size;
    };
}