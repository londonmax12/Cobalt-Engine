#pragma once

#include "Application/Core.h"

#include <stdint.h>

namespace Cobalt {
	namespace Platform
	{
		struct PlatformState
		{
			void* internalState;
		};
		bool Init();

		bool Startup(Ref<PlatformState> state, const char* applicationName, int positionX, int positionY, int width, int height);
		void Shutdown(Ref<PlatformState> state);

		bool PumpMessages(Ref<PlatformState> state);

		void* Allocate(uint32_t size, bool aligned);
		void Free(void* block, bool aligned);
		void* ZeroMem(void* block, uint32_t size);
		void* CopyMem(void* dest, const void* source, uint32_t size);
		void* SetMem(void* dest, int32_t value, uint32_t size);

		void ConsoleWrite(const char* msg, int color);
		void ConsoleWriteError(const char* msg, int color);

		double GetAbsTime();
		void Sleep(int ms);
	}
}