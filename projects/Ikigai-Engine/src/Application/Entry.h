#pragma once

#include "Application/Application.h"
#include "Logging/Logging.h"
#include "Input/Input.h"

extern Ikigai::Application* Ikigai::CreateApplication();

void main(int argc, char** argv) {
	Ikigai::Logging::Init();
	IKIGAI_INFO("Initialized logging system");

	Ikigai::Platform::Init();
	IKIGAI_INFO("Initialized platform layer");

	Ikigai::Input::Init();
	IKIGAI_INFO("Initialized platform layer");

	Ikigai::Application* app = Ikigai::CreateApplication();
	IKIGAI_INFO("Created application");
	app->Run();
	delete app;
}