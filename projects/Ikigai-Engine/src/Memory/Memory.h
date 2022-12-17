#pragma once
#include <cstdint>

namespace Ikigai {
	enum MemoryType
	{
		MEMORY_TYPE_OTHER,
		MEMORY_TYPE_RENDERER,

		MEMORYMAXTYPES,
	};
	
	class Memory {
	private:
		struct MemoryStats {
			uint64_t total;
			uint64_t typeAllocations[MEMORYMAXTYPES];
		};

		static inline MemoryStats m_Stats = MemoryStats();

		static const char* MemoryTypeToString(MemoryType type);
	public:
		static void Init();
		static void Shutdown();

		static void* Allocate(uint32_t size, MemoryType type);
		static void Free(void* block, uint32_t size, MemoryType type);
		static void* Zero(void* block, uint32_t size);
		static void* Copy(void* dest, const void* source, uint32_t size);
		static void* Set(void* dest, int32_t value, uint32_t size);

		static char* PrintPerformance();

	};
}