#include "Ikigai.h"

class SandboxApp : Ikigai::Application {
public:
	SandboxApp() {
		Ikigai::ApplicationConfig config = Ikigai::ApplicationConfig::createConfig();
		config.ApplicationName = "Sandbox";
		Init(config);
	}
	void Begin() {
		Run();
	}
};

void main() {
	SandboxApp app = SandboxApp();
	app.Begin();
}