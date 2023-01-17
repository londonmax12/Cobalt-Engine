#include "Ikigai.h"

class SandboxApp : public Ikigai::Application {
public:
	SandboxApp(Ikigai::ApplicationConfig conf)
		: Ikigai::Application(conf)
	{

	}

	void OnUpdate(Ikigai::DeltaTime deltaTime) { 
	}
};

Ikigai::Application* Ikigai::CreateApplication() {
	return new SandboxApp(ApplicationConfig("Sandbox", {1600, 900}, {100, 100}));
}