#include "ikipch.h"
#include "Input.h"
#include "Platforms/PlatformInput.h"

bool Ikigai::Input::Init()
{
    PlatformInput::Init();

    return true;
}

bool Ikigai::Input::IsMouseButtonDown(int mouseButton)
{
    return PlatformInput::IsMouseButtonDown(mouseButton);
}
