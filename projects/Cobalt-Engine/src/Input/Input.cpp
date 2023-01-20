#include "cobaltpch.h"
#include "Input.h"
#include "Platforms/PlatformInput.h"

bool Cobalt::Input::Init()
{
    PlatformInput::Init();

    return true;
}

bool Cobalt::Input::IsMouseButtonDown(int mouseButton)
{
    return PlatformInput::IsMouseButtonDown(mouseButton);
}
