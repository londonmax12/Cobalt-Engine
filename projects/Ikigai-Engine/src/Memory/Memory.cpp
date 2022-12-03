#include "ikipch.h"
#include "Memory.h"
#include "Platforms/Platform.h"

const char* Ikigai::Memory::MemoryTypeToString(MemoryType type)
{
	switch (type) {
		case MemoryType::MEMORY_TYPE_OTHER:
		{
			return "Other:   ";
		}
		default:
		{
			return "Unknown: ";
		}
	}
}

void Ikigai::Memory::MemInit()
{
}

void Ikigai::Memory::MemShutdown()
{
}

void* Ikigai::Memory::MemAllocate(uint64_t size, MemoryType type)
{
	m_Stats.total += size;
	m_Stats.typeAllocations[type] += size;

	void* block = platform_Allocate(size, false);
	platform_ZeroMemory(block, size);
	return block;
}

void Ikigai::Memory::MemFree(void* block, uint64_t size, MemoryType type)
{
	m_Stats.total -= size;
	m_Stats.typeAllocations[type] -= size;

	platform_Free(block, false);
}

void* Ikigai::Memory::MemZeroMemory(void* block, uint64_t size)
{
	return platform_ZeroMemory(block, size);
}

void* Ikigai::Memory::MemCopyMemory(void* dest, const void* source, uint64_t size)
{
	return platform_CopyMemory(dest, source, size);
}

void* Ikigai::Memory::MemSetMemory(void* dest, int32_t value, uint64_t size)
{
	return platform_SetMemory(dest, value, size);
}

char* Ikigai::Memory::PrintPerformance()
{
	const uint64_t kb = 1024;
	const uint64_t mb = 1048576;
	const uint64_t gb = 1073741824;

	IKIGAI_INFO("====== System Memory Usage ======");
	for (int i = 0; i < MEMORYMAXTYPES; i++) {
		const char* unit = "B";
		float amount = 0.0f;
		if (m_Stats.typeAllocations[i] >= kb) {
			unit = "KB";
			amount = m_Stats.typeAllocations[i] / kb;
		}
		if (m_Stats.typeAllocations[i] >= mb) {
			unit = "MB";
			amount = m_Stats.typeAllocations[i] / mb;
		}
		else if (m_Stats.typeAllocations[i] >= gb) {
			unit = "GB";
			amount = m_Stats.typeAllocations[i] / gb;
		}
		else {
			amount = m_Stats.typeAllocations[i];
		}
		const char* label = MemoryTypeToString((MemoryType)i);
		IKIGAI_INFO("%s%f", label, amount);
		
	}
	IKIGAI_INFO("=================================");
	return nullptr;
}
