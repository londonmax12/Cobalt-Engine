#pragma once

#include <Windows.h>

namespace Cobalt {
	struct WindowsInternalState {
		HINSTANCE hInstance = nullptr;
		HWND hwnd = nullptr;
	};
}