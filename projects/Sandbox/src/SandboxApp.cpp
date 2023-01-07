#include "Ikigai.h"

class SandboxApp : public Ikigai::Application {
public:
	SandboxApp() {
		Ikigai::ApplicationConfig config = Ikigai::ApplicationConfig::createConfig();

		config.ApplicationName = "Sandbox";

		Init(config);
	}

	void OnUpdate(Ikigai::DeltaTime deltaTime) { 
		IKIGAI_INFO("{}", deltaTime.Elapsed());
	}
};

Ikigai::Application* Ikigai::CreateApplication() {
	return new SandboxApp();
}