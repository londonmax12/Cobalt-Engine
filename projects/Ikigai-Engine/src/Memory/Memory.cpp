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

void Ikigai::Memory::Init()
{
}

void Ikigai::Memory::Shutdown()
{
}

void* Ikigai::Memory::Allocate(uint32_t size, MemoryType type)
{
	m_Stats.total += size;
	m_Stats.typeAllocations[type] += size;

	void* block = Platform::Allocate(size, false);
	Platform::ZeroMem(block, size);
	return block;
}

void Ikigai::Memory::Free(void* block, uint32_t size, MemoryType type)
{
	m_Stats.total -= size;
	m_Stats.typeAllocations[type] -= size;

	Platform::Free(block, false);
}

void* Ikigai::Memory::Zero(void* block, uint32_t size)
{
	return Platform::ZeroMem(block, size);
}

void* Ikigai::Memory::Copy(void* dest, const void* source, uint32_t size)
{
	return Platform::CopyMem(dest, source, size);
}

void* Ikigai::Memory::Set(void* dest, int32_t value, uint32_t size)
{
	return Platform::SetMem(dest, value, size);
}

char* Ikigai::Memory::PrintPerformance()
{
	const float kb = 1024.f;
	const float mb = 1048576.f;
	const float gb = 1073741824.f;

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
			amount = (float)m_Stats.typeAllocations[i];
		}
		const char* label = MemoryTypeToString((MemoryType)i);
		IKIGAI_INFO("%s%f", label, amount);
		
	}
	IKIGAI_INFO("=================================");
	return nullptr;
}

//void* Ikigai::Memory::ZeroMemory(void* block, uint64_t size)
//{
//	return Platform::ZeroMem(block, size);
//}
//
//void* Ikigai::Memory::CopyMemory(void* dest, const void* source, uint64_t size)
//{
//	return Platform::CopyMem(dest, source, size);
//}
//
//void* Ikigai::Memory::SetMemory(void* dest, int32_t value, uint64_t size)
//{
//	return Platform::SetMem(dest, value, size);
//}