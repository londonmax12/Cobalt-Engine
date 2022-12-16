#include "Ikigai.h"

bool ResizeEvent(Ikigai::Event& ev) {
	IKIGAI_DEBUG("Resize!");
	return true;
}

class SandboxApp : public Ikigai::Application {
public:
	SandboxApp() {
		Ikigai::ApplicationConfig config = Ikigai::ApplicationConfig::createConfig();
		config.ApplicationName = "Sandbox";
		Init(config);

		Ikigai::EventListener windowResizeEvent{ Ikigai::EVENTTYPE_WINDOW_RESIZE, &ResizeEvent };
		Ikigai::EventListener windowResizeEvent1{ Ikigai::EVENTTYPE_WINDOW_RESIZE, &ResizeEvent1 };
		Ikigai::EventSystem::Register(windowResizeEvent);	
		//Ikigai::EventSystem::Unregister(windowResizeEvent);	
	}
};

Ikigai::Application* Ikigai::CreateApplication() {
	return new SandboxApp();
} 