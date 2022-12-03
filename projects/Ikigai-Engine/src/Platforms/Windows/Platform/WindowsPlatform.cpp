#include "ikipch.h"

#ifdef IKIGAI_PLATFORM_WINDOWS
#include "Platforms/Platform.h"
#endif


namespace Ikigai {
	static double g_ClockFrequency;
	static LARGE_INTEGER g_StartTime;
}

void Ikigai::platform_Init()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	g_ClockFrequency = 1.0 / (float)freq.QuadPart;
	QueryPerformanceCounter(&g_StartTime);
}

bool Ikigai::platform_PumpMessages(PlatformState* state)
{
    MSG msg;
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return true;
}

void* Ikigai::platform_Allocate(uint32_t size, bool aligned) {
	return malloc(size);
}
void Ikigai::platform_Free(void* block, bool aligned) {
	free(block);
}
void* Ikigai::platform_ZeroMemory(void* block, uint32_t size)
{
	return memset(block, 0, size);
}
void* Ikigai::platform_CopyMemory(void* dest, const void* source, uint32_t size)
{
	return memcpy(dest, source, size);
}
void* Ikigai::platform_SetMemory(void* dest, int32_t value, uint32_t size)
{
	return memset(dest, value, size);
}
void Ikigai::platform_ConsoleWrite(const char* msg, int color) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	uint32_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
void Ikigai::platform_ConsoleWriteError(const char* msg, int color)
{
	HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	uint32_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
double Ikigai::platform_GetAbsTime()
{
	LARGE_INTEGER timeNow;
	QueryPerformanceCounter(&timeNow);
	return (double)timeNow.QuadPart * g_ClockFrequency;
}
void Ikigai::platform_Sleep(int ms)
{
	Sleep(ms);
}