#include "Ikigai.h"

bool ResizeEvent(Ikigai::Ref<Ikigai::Event> ev) {
	Ikigai::SurfaceResizeEvent* resizeEvent = (Ikigai::SurfaceResizeEvent*)ev.get();
	IKIGAI_DEBUG("Resize! {}, {}", resizeEvent->Width, resizeEvent->Height);
	return true;
}

class SandboxApp : public Ikigai::Application {
public:
	SandboxApp() {
		Ikigai::ApplicationConfig config = Ikigai::ApplicationConfig::createConfig();
		config.ApplicationName = "Sandbox";
		Init(config);

		Ikigai::EventListener windowResizeEvent{ Ikigai::EVENTTYPE_SURFACE_RESIZE, &ResizeEvent };
		Ikigai::EventSystem::Register(windowResizeEvent);	
		//Ikigai::EventSystem::Unregister(windowResizeEvent);	
	}
};

Ikigai::Application* Ikigai::CreateApplication() {
	return new SandboxApp();
}