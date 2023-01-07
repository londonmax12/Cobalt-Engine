#include "ikipch.h"

#ifdef IKIGAI_PLATFORM_WINDOWS
#include "Platforms/Platform.h"
#include "Platforms/Windows/WindowsStructs.h"

#include "Event/SurfaceEvents.h"
#include "Event/ApplicationEvents.h"
#include "Event/MouseEvents.h"

#include "Input/Keycodes.h"
#include "Platforms/PlatformInput.h"

namespace Ikigai {
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
			EventSystem::PushEvent(CreateRef<ApplicationCloseEvent>());
			return 1;
		}

		case WM_DESTROY:
		{
			EventSystem::PushEvent(CreateRef<ApplicationCloseEvent>());
			PostQuitMessage(0);
			return 1;
		}

		case WM_SIZE:
		{
			RECT r;
			GetClientRect(hwnd, &r);
			int width = r.right - r.left;
			int height = r.bottom - r.top;
			EventSystem::PushEvent(CreateRef<SurfaceResizeEvent>(width, height));
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
			
			EventSystem::PushEvent(CreateRef<MouseScrollEvent>(zDelta));
			break;
		}

		case WM_LBUTTONDOWN:
		{
			EventSystem::PushEvent(CreateRef<MouseDownEvent>(M_LEFT));
			break;
		}
		case WM_RBUTTONDOWN:
		{
			EventSystem::PushEvent(CreateRef<MouseDownEvent>(M_RIGHT));
			break;
		}
		case WM_MBUTTONDOWN:
		{
			EventSystem::PushEvent(CreateRef<MouseDownEvent>(M_MIDDLE));
			break;
		}
		case WM_LBUTTONUP:
		{
			EventSystem::PushEvent(CreateRef<MouseUpEvent>(M_LEFT));
			break;
		}
		case WM_RBUTTONUP:
		{
			EventSystem::PushEvent(CreateRef<MouseUpEvent>(M_RIGHT));
			break;
		}
		case WM_MBUTTONUP:
		{
			EventSystem::PushEvent(CreateRef<MouseUpEvent>(M_MIDDLE));
			break;
		}
		case WM_XBUTTONDOWN:
		{
			EventSystem::PushEvent(CreateRef<MouseDownEvent>(GET_XBUTTON_WPARAM(wParam) == 1 ? M_S1 : M_S2));
			break;
		}
		case WM_XBUTTONUP:
		{
			EventSystem::PushEvent(CreateRef<MouseUpEvent>(GET_XBUTTON_WPARAM(wParam) == 1 ? M_S1 : M_S2));
			break;
		}
		default:
			break;
		}
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
}

void Ikigai::Platform::Init()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	g_ClockFrequency = 1.0 / (float)freq.QuadPart;
	QueryPerformanceCounter(&g_StartTime);
}

bool Ikigai::Platform::Startup(PlatformState* state, const char* applicationName, int positionX, int positionY, int width, int height)
{
	state->internalState = malloc(sizeof(WindowsInternalState));
	WindowsInternalState* internalState = (WindowsInternalState*)state->internalState;

	if (internalState == nullptr)
		return false;

	internalState->hInstance = GetModuleHandleA(0);

	HICON icon = LoadIcon(internalState->hInstance, IDI_APPLICATION);
	WNDCLASSA wc;
	ZeroMem(&wc, sizeof(wc));

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = Win32MsgProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = internalState->hInstance;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszClassName = "IkigaiWindowClass";

	if (!RegisterClassA(&wc)) {
		MessageBoxA(0, "Failed to register window class", "Error", MB_ICONEXCLAMATION | MB_OK);
		IKIGAI_FATAL("Failed to register window class");
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

	HWND handle = CreateWindowExA(windowExStyle, "IkigaiWindowClass", applicationName, windowStyle, wX, wY, wW, wH, 0, 0, internalState->hInstance, 0);

	if (handle == 0) {
		MessageBoxA(0, "Failed to create window", "Error", MB_ICONEXCLAMATION | MB_OK);
		IKIGAI_FATAL("Failed to create window");
		return false;
	}

	IKIGAI_INFO("Created Win32 window ({})", applicationName);
	IKIGAI_INFO("\t| Pos: {}", Vector2(positionX, positionY));
	IKIGAI_INFO("\t| Size: {}", Vector2(width, height));

	internalState->hwnd = handle;

	bool shouldActivate = true;
	int showWindowCommandFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;

	ShowWindow(internalState->hwnd, showWindowCommandFlags);

	return true;
}

void Ikigai::Platform::Shutdown(PlatformState* state)
{
	WindowsInternalState* internalState = (WindowsInternalState*)state->internalState;
	if (internalState->hwnd) {
		DestroyWindow(internalState->hwnd);
	}
}

bool Ikigai::Platform::PumpMessages(PlatformState* state)
{
    MSG msg;
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return true;
}

void* Ikigai::Platform::Allocate(uint32_t size, bool aligned) {
	return malloc(size);
}
void Ikigai::Platform::Free(void* block, bool aligned) {
	free(block);
}
void* Ikigai::Platform::ZeroMem(void* block, uint32_t size) {
	return memset(block, 0, size);
}
void* Ikigai::Platform::CopyMem(void* dest, const void* source, uint32_t size)
{
	return memcpy(dest, source, size);
}
void* Ikigai::Platform::SetMem(void* dest, int32_t value, uint32_t size) {
	return memset(dest, value, size);
}
void Ikigai::Platform::ConsoleWrite(const char* msg, int color) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	size_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
void Ikigai::Platform::ConsoleWriteError(const char* msg, int color) {
	HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
	static int levels[6] = { 8, 1, 7, 6, 4, 12 };
	SetConsoleTextAttribute(consoleHandle, levels[color]);
	OutputDebugStringA(msg);
	size_t len = strlen(msg);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)len, numberWritten, 0);
}
double Ikigai::Platform::GetAbsTime() {
	LARGE_INTEGER timeNow;
	QueryPerformanceCounter(&timeNow);
	return (double)timeNow.QuadPart * g_ClockFrequency;
}
void Ikigai::Platform::Sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
#endif