#pragma once

#include <Windows.h>

namespace Ikigai {
	struct WindowsInternalState {
		HINSTANCE hInstance = nullptr;
		HWND hwnd = nullptr;
	};
}