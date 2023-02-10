#include "Cobalt.h"

#include "SandboxLayer.h"

class SandboxApp : public Cobalt::Application {
public:
	SandboxApp(Cobalt::ApplicationConfig conf)
		: Cobalt::Application(conf)
	{
		PushLayer(Cobalt::CreateRef<SandboxLayer>());
	}
};

Cobalt::Application* Cobalt::CreateApplication() {
	return new SandboxApp(ApplicationConfig("Sandbox", {1600, 900}, {100, 100}));
}