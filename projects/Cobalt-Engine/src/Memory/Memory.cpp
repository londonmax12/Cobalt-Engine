#include "cobaltpch.h"
#include "Memory.h"
#include "Platforms/Platform.h"

const char* Cobalt::Memory::MemoryTypeToString(MemoryType type)
{
	switch (type) {
		case MemoryType::MEMORY_TYPE_OTHER:
		{
			return "Other";
		}
		case MemoryType::MEMORY_TYPE_RENDERER:
		{
			return "Renderer";
		}
		case MemoryType::MEMORY_TYPE_POOL_ALLOCATOR:
		{
			return "Pool Allocator";
		}
		default:
		{
			return "Unknown";
		}
	}
}

void* Cobalt::Memory::Allocate(uint32_t size, MemoryType type)
{
	m_Stats.total += size;
	m_Stats.typeAllocations[type] += size;

	void* block = Platform::Allocate(size, false);
	Platform::ZeroMem(block, size);
	return block;
}

void Cobalt::Memory::Free(void* block, uint32_t size, MemoryType type)
{
	m_Stats.total -= size;
	m_Stats.typeAllocations[type] -= size;

	Platform::Free(block, false);
}

void* Cobalt::Memory::Zero(void* block, uint32_t size)
{
	return Platform::ZeroMem(block, size);
}

void* Cobalt::Memory::Copy(void* dest, const void* source, uint32_t size)
{
	return Platform::CopyMem(dest, source, size);
}

void* Cobalt::Memory::Set(void* dest, int32_t value, uint32_t size)
{
	return Platform::SetMem(dest, value, size);
}

char* Cobalt::Memory::PrintPerformance()
{
	const float kb = 1024.f;
	const float mb = 1048576.f;
	const float gb = 1073741824.f;

	COBALT_INFO("System Memory Usage ({})", m_Stats.total);
	for (int i = 0; i < MemoryType::MEMORY_MAX_TYPES; i++) {
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
		COBALT_INFO("\t| {}: {}", label, amount);	
	}
	return nullptr;
}