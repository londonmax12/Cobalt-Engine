#include "Cobalt.h"

class SandboxApp : public Cobalt::Application {
public:
	SandboxApp(Cobalt::ApplicationConfig conf)
		: Cobalt::Application(conf)
	{

	}

	void OnUpdate(Cobalt::DeltaTime deltaTime) {
	}
};

Cobalt::Application* Cobalt::CreateApplication() {
	return new SandboxApp(ApplicationConfig("Sandbox", {1600, 900}, {100, 100}));
}