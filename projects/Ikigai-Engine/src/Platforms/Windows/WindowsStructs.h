#pragma once

#include <Windows.h>

namespace Ikigai {
	struct WindowsInternalState {
		HINSTANCE hInstance;
		HWND hwnd;
	};
}