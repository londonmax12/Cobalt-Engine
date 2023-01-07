
#include "ikipch.h"

#ifdef IKIGAI_PLATFORM_WINDOWS
#include "Input/Keycodes.h"
#include "Platforms/PlatformInput.h"

namespace Ikigai {
	static std::unordered_map<int, KeyCode> s_WindowsKeyCodes{};
	static std::unordered_map<int, MouseCode> s_WindowsMouseCodes{};

	KeyCode PlatformInput::GetKeyCode(int platformKey)
	{
		if (s_WindowsKeyCodes.find(platformKey) == s_WindowsKeyCodes.end())
			return K_UNKNOWN;

		return s_WindowsKeyCodes[platformKey];
	}

	MouseCode PlatformInput::GetMouseCode(int mouseButton)
	{
		
		if (s_WindowsMouseCodes.find(mouseButton) == s_WindowsMouseCodes.end())
			return M_UNKNOWN;

		MouseCode mc = s_WindowsMouseCodes[mouseButton];
		return mc;
	}

	bool PlatformInput::IsMouseButtonDown(int mouseButton)
	{
		if (mouseButton > 5)
			return false;

		return GetAsyncKeyState(mouseButton);
	}

	void PlatformInput::Init()
	{
		s_WindowsKeyCodes[VK_ESCAPE] = K_ESCAPE;

		s_WindowsKeyCodes[VK_F1] = K_F1;
		s_WindowsKeyCodes[VK_F2] = K_F2;
		s_WindowsKeyCodes[VK_F3] = K_F3;
		s_WindowsKeyCodes[VK_F4] = K_F4;
		s_WindowsKeyCodes[VK_F5] = K_F5;
		s_WindowsKeyCodes[VK_F6] = K_F6;
		s_WindowsKeyCodes[VK_F7] = K_F7;
		s_WindowsKeyCodes[VK_F8] = K_F8;
		s_WindowsKeyCodes[VK_F9] = K_F9;
		s_WindowsKeyCodes[VK_F10] = K_F10;
		s_WindowsKeyCodes[VK_F11] = K_F11;
		s_WindowsKeyCodes[VK_F12] = K_F12;

		s_WindowsKeyCodes[VK_PRINT] = K_PRINTSCRN;
		s_WindowsKeyCodes[VK_SCROLL] = K_SCRLK;
		s_WindowsKeyCodes[VK_NUMLOCK] = K_NUMLK;
		s_WindowsKeyCodes[VK_PAUSE] = K_PAUSE;

		s_WindowsKeyCodes[VK_SPACE] = K_SPACE;

		s_WindowsKeyCodes[VK_OEM_5] = K_APOSTROPHE;
		s_WindowsKeyCodes[VK_OEM_COMMA] = K_COMMA;

		s_WindowsKeyCodes[48] = K_0;
		s_WindowsKeyCodes[49] = K_1;
		s_WindowsKeyCodes[50] = K_2;
		s_WindowsKeyCodes[51] = K_3;
		s_WindowsKeyCodes[52] = K_4;
		s_WindowsKeyCodes[53] = K_5;
		s_WindowsKeyCodes[54] = K_6;
		s_WindowsKeyCodes[55] = K_7;
		s_WindowsKeyCodes[56] = K_8;
		s_WindowsKeyCodes[57] = K_9;

		s_WindowsKeyCodes[VK_NUMPAD0] = K_NUM0;
		s_WindowsKeyCodes[VK_NUMPAD1] = K_NUM1;
		s_WindowsKeyCodes[VK_NUMPAD2] = K_NUM2;
		s_WindowsKeyCodes[VK_NUMPAD3] = K_NUM3;
		s_WindowsKeyCodes[VK_NUMPAD4] = K_NUM4;
		s_WindowsKeyCodes[VK_NUMPAD5] = K_NUM5;
		s_WindowsKeyCodes[VK_NUMPAD6] = K_NUM6;
		s_WindowsKeyCodes[VK_NUMPAD7] = K_NUM7;
		s_WindowsKeyCodes[VK_NUMPAD8] = K_NUM8;
		s_WindowsKeyCodes[VK_NUMPAD9] = K_NUM9;

		s_WindowsKeyCodes[65] = K_A;
		s_WindowsKeyCodes[66] = K_B;
		s_WindowsKeyCodes[67] = K_C;
		s_WindowsKeyCodes[68] = K_D;
		s_WindowsKeyCodes[69] = K_E;
		s_WindowsKeyCodes[70] = K_F;
		s_WindowsKeyCodes[71] = K_G;
		s_WindowsKeyCodes[72] = K_H;
		s_WindowsKeyCodes[73] = K_I;
		s_WindowsKeyCodes[74] = K_J;
		s_WindowsKeyCodes[75] = K_K;
		s_WindowsKeyCodes[76] = K_L;
		s_WindowsKeyCodes[77] = K_M;
		s_WindowsKeyCodes[78] = K_N;
		s_WindowsKeyCodes[79] = K_O;
		s_WindowsKeyCodes[80] = K_P;
		s_WindowsKeyCodes[81] = K_Q;
		s_WindowsKeyCodes[82] = K_R;
		s_WindowsKeyCodes[83] = K_S;
		s_WindowsKeyCodes[84] = K_T;
		s_WindowsKeyCodes[85] = K_U;
		s_WindowsKeyCodes[86] = K_V;
		s_WindowsKeyCodes[87] = K_W;
		s_WindowsKeyCodes[88] = K_X;
		s_WindowsKeyCodes[89] = K_Y;
		s_WindowsKeyCodes[90] = K_Z;

		s_WindowsKeyCodes[VK_LEFT] = K_LEFT;
		s_WindowsKeyCodes[VK_RIGHT] = K_RIGHT;
		s_WindowsKeyCodes[VK_UP] = K_UP;
		s_WindowsKeyCodes[VK_DOWN] = K_DOWN;

		s_WindowsKeyCodes[VK_INSERT] = K_SPACE;
		s_WindowsKeyCodes[VK_HOME] = K_HOME;
		s_WindowsKeyCodes[VK_DELETE] = K_DELETE;
		s_WindowsKeyCodes[VK_END] = K_END;
		s_WindowsKeyCodes[VK_PRIOR] = K_PAGEUP;
		s_WindowsKeyCodes[VK_NEXT] = K_PAGEDOWN;

		s_WindowsKeyCodes[VK_BACK] = K_BACKSPACE;
		s_WindowsKeyCodes[VK_TAB] = K_TAB;
		s_WindowsKeyCodes[VK_RETURN] = K_RETURN;
		s_WindowsKeyCodes[VK_CAPITAL] = K_CAPSLOCK;
		s_WindowsKeyCodes[VK_APPS] = K_MENU;

		s_WindowsKeyCodes[VK_SHIFT] = K_SHIFT;
		s_WindowsKeyCodes[VK_CONTROL] = K_CTRL;
		s_WindowsKeyCodes[VK_MENU] = K_ALT;

		s_WindowsKeyCodes[VK_LSHIFT] = K_LSHIFT;
		s_WindowsKeyCodes[VK_LCONTROL] = K_LCTRL;
		s_WindowsKeyCodes[VK_LWIN] = K_LSUPER;
		s_WindowsKeyCodes[VK_LMENU] = K_LALT;

		s_WindowsKeyCodes[VK_RSHIFT] = K_RSHIFT;
		s_WindowsKeyCodes[VK_RCONTROL] = K_RCTRL;
		s_WindowsKeyCodes[VK_RWIN] = K_RSUPER;
		s_WindowsKeyCodes[VK_RMENU] = K_RALT;

		s_WindowsKeyCodes[VK_VOLUME_MUTE] = K_MUTE;
		s_WindowsKeyCodes[VK_VOLUME_UP] = K_VOLUMEUP;
		s_WindowsKeyCodes[VK_VOLUME_DOWN] = K_VOLUMEDOWN;

		s_WindowsKeyCodes[VK_BROWSER_BACK] = K_BACK;
		s_WindowsKeyCodes[VK_BROWSER_FORWARD] = K_FOWARD;

		s_WindowsKeyCodes[VK_OEM_PERIOD] = K_PERIOD;
		s_WindowsKeyCodes[VK_OEM_MINUS] = K_MINUS;
		s_WindowsKeyCodes[VK_OEM_PLUS] = K_PLUS;

		s_WindowsKeyCodes[VK_OEM_4] = K_EXTRA1;
		s_WindowsKeyCodes[VK_OEM_6] = K_EXTRA2;
		s_WindowsKeyCodes[VK_OEM_2] = K_EXTRA3;
		s_WindowsKeyCodes[VK_OEM_7] = K_EXTRA4;
		s_WindowsKeyCodes[VK_OEM_3] = K_EXTRA5;
		s_WindowsKeyCodes[VK_OEM_1] = K_EXTRA6;

		s_WindowsKeyCodes[255] = K_FN;


		s_WindowsMouseCodes[VK_LBUTTON] = M_LEFT;
		s_WindowsMouseCodes[VK_RBUTTON] = M_RIGHT;
		s_WindowsMouseCodes[VK_MBUTTON] = M_MIDDLE;
		s_WindowsMouseCodes[VK_XBUTTON1] = M_S1;
		s_WindowsMouseCodes[VK_XBUTTON2] = M_S2;
	}
}
#endif