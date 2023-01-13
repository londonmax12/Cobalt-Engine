#pragma once

#include "Application/Application.h"
#include "Logging/Logging.h"
#include "Input/Input.h"

extern Ikigai::Application* Ikigai::CreateApplication();

int main(int argc, char** argv) {
	Ikigai::Logging::Init();
	IKIGAI_INFO("Initialized logging system");

	Ikigai::Platform::Init();
	IKIGAI_INFO("Initialized platform layer");

	Ikigai::Input::Init();
	IKIGAI_INFO("Initialized input system");

	Ikigai::Application* app = Ikigai::CreateApplication();
	if (!app->IsInitialized()) {
		IKIGAI_WARN("Application config was not supplied, using default");
		app->Init(Ikigai::ApplicationConfig());
	}
	IKIGAI_INFO("Created application");
	app->Run();
	delete app;
}