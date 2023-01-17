#include "ikipch.h"
#include "PoolAllocator.h"

namespace Ikigai {

	template<typename T>
	PoolAllocator<T>::PoolAllocator(size_t size)
		: m_Size(size)
	{
		m_Buffer = static_cast<char*>(Memory::Allocate(size * sizeof(T), MemoryType::MEMORY_TYPE_POOL_ALLOCATOR));
		m_FreeList = static_cast<void**>(Memory::Allocate(size * sizeof(void*), MemoryType::MEMORY_TYPE_POOL_ALLOCATOR));

		for (int i = 0; i < size; i++) {
			m_FreeList[i] = m_Buffer + i * sizeof(T);
		}
	}

	template<typename T>
	PoolAllocator<T>::~PoolAllocator()
	{
		delete[] m_Buffer;
		delete[] m_FreeList;
	}

	template<typename T>
	T* PoolAllocator<T>::Allocate()
	{
		if (m_NextFree == 0) {
			return nullptr;
		}
		void* result = m_FreeList[m_NextFree--];
		return new (result) T();  
	}

	template<typename T>
	void PoolAllocator<T>::Deallocate(T* p)
	{
		p->~T();
		m_FreeList[m_NextFree++] = p;
	}
}