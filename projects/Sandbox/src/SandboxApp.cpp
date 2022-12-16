#include "Ikigai.h"

class SandboxApp : public Ikigai::Application {
public:
	SandboxApp() {
		Ikigai::ApplicationConfig config = Ikigai::ApplicationConfig::createConfig();
		config.ApplicationName = "Sandbox";
		Init(config);
	}
};

Ikigai::Application* Ikigai::CreateApplication() {
	return new SandboxApp();
}