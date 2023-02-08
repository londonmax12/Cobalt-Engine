#include "cobaltpch.h"

#ifdef COBALT_PLATFORM_WINDOWS
#include "Platforms/Platform.h"
#include "Platforms/Windows/WindowsStructs.h"

#include "Event/SurfaceEvents.h"
#include "Event/ApplicationEvents.h"
#include "Event/MouseEvents.h"

#include "Input/Keycodes.h"

#include "Platforms/PlatformInput.h"

#include "Application/Application.h"

namespace Cobalt {
	static double g_ClockFrequency;
	static LARGE_INTEGER g_StartTime;

	LRESULT CALLBACK Win32MsgProcess(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam) {
		switch (msg)
		{
		case WM_ERASEBKGND:
		{
			return 1;
		}

		case WM_CLOSE:
		{
			ApplicationCloseEvent ev = ApplicationCloseEvent();
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			return 1;
		}

		case WM_DESTROY:
		{
			ApplicationCloseEvent ev = ApplicationCloseEvent();
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			PostQuitMessage(0);
			return 1;
		}

		case WM_SIZE:
		{
			RECT r;
			GetClientRect(hwnd, &r);
			int width = r.right - r.left;
			int height = r.bottom - r.top;
			SurfaceResizeEvent ev = SurfaceResizeEvent(width, height);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			break;
		}

		case WM_MOUSEMOVE:
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			break;
		}

		case WM_MOUSEWHEEL:
		{
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			
			if (zDelta != 0) {
				zDelta = (zDelta < 0) ? -1 : 1;
			}
			
			MouseScrolledEvent ev = MouseScrolledEvent((float)zDelta, 0);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}

		case WM_LBUTTONDOWN:
		{

			MouseDownEvent ev = MouseDownEvent(M_LEFT);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_RBUTTONDOWN:
		{
			MouseDownEvent ev = MouseDownEvent(M_RIGHT);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_MBUTTONDOWN:
		{
			MouseDownEvent ev = MouseDownEvent(M_MIDDLE);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_XBUTTONDOWN:
		{
			MouseDownEvent ev = MouseDownEvent(GET_XBUTTON_WPARAM(wParam) == 1 ? M_S1 : M_S2);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_LBUTTONUP:
		{
			MouseUpEvent ev = MouseUpEvent(M_LEFT);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_RBUTTONUP:
		{
			MouseUpEvent ev = MouseUpEvent(M_RIGHT);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_MBUTTONUP:
		{
			MouseUpEvent ev = MouseUpEvent(M_MIDDLE);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		case WM_XBUTTONUP:
		{
			MouseUpEvent ev = MouseUpEvent(GET_XBUTTON_WPARAM(wParam) == 1 ? M_S1 : M_S2);
			EventSystem::GetInstance()->DispatchEvent(static_cast<Event&>(ev));
			break;
		}
		default:
			break;
		}
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}

bool Cobalt::Platform::Init()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	g_ClockFrequency = 1.0 / (float)freq.QuadPart;
	QueryPerformanceCounter(&g_StartTime);
	return true;
}

bool Cobalt::Platform::Startup(Ref<Platform::PlatformState> state, const char* applicationName, int positionX, int positionY, int width, int height)
{
	state->internalState = malloc(sizeof(WindowsInternalState));
	WindowsInternalState* internalState = (WindowsInternalState*)&state->internalState;

	if (internalState == nullptr)
		return false;

	internalState->hInstance = GetModuleHandleA(0);

	HICON icon = LoadIcon(internalState->hInstance, IDI_APPLICATION);
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = Win32MsgProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = internalState->hInstance;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = "CobaltWindowClass";

	if (!RegisterClassA(&wc)) {
		MessageBoxA(0, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
		COBALT_FATAL("Failed to register window class");
		return false;
	}

	int cX = positionX;
	int cY = positionY;
	int cW = width;
	int cH = height;

	int wX = cX;
	int wY = cY;
	int wW = cW;
	int wH = cH;

	int windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	int windowExStyle = WS_EX_APPWINDOW;

	windowStyle |= WS_MAXIMIZEBOX;
	windowStyle |= WS_MINIMIZEBOX;
	windowStyle |= WS_THICKFRAME;

	RECT bRect = { 0,0,0,0 };
	AdjustWindowRect(&bRect, windowStyle, false);

	wX += bRect.left;
	wY += bRect.top;

	wW += bRect.right - bRect.left;
	wH += bRect.bottom - bRect.top;

	HWND handle = CreateWindowExA(windowExStyle, "CobaltWindowClass", applicationName, windowStyle, wX, wY, wW, wH, 0, 0, internalState->hInstance, 0);

	if (handle == 0) {
		MessageBoxA(0, "Failed to create window", "Error", MB_ICONEXCLAMATION | MB_OK);
		COBALT_FATAL("Failed to create window");
		return false;
	}

	COBALT_INFO("Created Win32 window ({})", applicationName);
	COBALT_INFO("\t| Pos: {}", Vector2(positionX, positionY));
	COBALT_INFO("\t| Size: {}", Vector2(width, height));
	COBALT_INFO("\t| Handle: {}", (uint16_t)handle);

	internalState->hwnd = handle;
	bool shouldActivate = true;
	int showWindowCommandFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;

	ShowWindow(internalState->hwnd, showWindowCommandFlags);

	return true;
}

void Cobalt::Platform::Shutdown(Ref<Platform::PlatformState> state)
{
	WindowsInternalState* internalState = (WindowsInternalState*)state->internalState;
	if (internalState == nullptr) {
		COBALT_WARN("Internal state was invalid while trying to shutdown Win32 platform");
		return;
	}
	if (internalState->hwnd) {
		DestroyWindow(internalState->hwnd);
	}
}

bool Cobalt::Platform::PumpMessages(Ref<Platform::PlatformState> state)
{
    MSG msg;
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return true;
}

void* Cobalt::Platform::Allocate(uint32_t size, bool aligned) {
	return malloc(size);
}
void Cobalt::Platform::Free(void* block, bool aligned) {
	free(block);
}
void* Cobalt::Platform::ZeroMem(void* block, uint32_t size) {
	return memset(block, 0, size);
}
void* Cobalt::Platform::CopyMem(void* dest, const void* source, uint32_t size)
{
	return memcpy(dest, source, size);
}
void* Cobalt::Platform::SetMem(void* dest, int32_t value, uint32_t size) {
	return memset(dest, value, size);
}
void Cobalt::Platform::ConsoleWrite(const char* msg, int color) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	size_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
void Cobalt::Platform::ConsoleWriteError(const char* msg, int color) {
	HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	size_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
double Cobalt::Platform::GetAbsTime() {
	LARGE_INTEGER timeNow;
	QueryPerformanceCounter(&timeNow);
	return (double)timeNow.QuadPart * g_ClockFrequency;
}
void Cobalt::Platform::Sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
#endif