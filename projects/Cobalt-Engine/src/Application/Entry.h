#pragma once

#include "Application/Application.h"
#include "Logging/Logging.h"
#include "Logging/Assert.h"
#include "Input/Input.h"
#include "Event/Event.h"

extern Cobalt::Application* Cobalt::CreateApplication();

int main(int argc, char** argv) {
	Cobalt::Logging::Init();
	COBALT_INFO("Initialized logging system");

	Cobalt::Input::Init();
	COBALT_INFO("Initialized input system");

	{
		bool success = Cobalt::EventSystem::Init();
		COBALT_ASSERT_MSG(success, "Failed to start event system!");
		if (!success)
			return false;
	}
	COBALT_INFO("Initialized event system");

	Cobalt::Application* app = Cobalt::CreateApplication();
	if (!app->IsInitialized()) {
		COBALT_WARN("Application config was not supplied, using default");
		app->Init(Cobalt::ApplicationConfig());
	}
	COBALT_INFO("Created application");
	app->Run();
	delete app;
}