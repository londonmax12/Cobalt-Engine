#include "Ikigai.h"

void main() {
	Ikigai::Logging::Init();
	IKIGAI_INFO("Initialized logging system");

	Ikigai::Platform::Init();
	IKIGAI_INFO("Initialized platform layer");

	Ikigai::Platform::PlatformState state{};
	IKIGAI_ASSERT_MSG(Ikigai::Platform::Startup(&state, "Sandbox", {30, 30}, {1000, 700}), "Failed to start application!");
	IKIGAI_INFO("Created platform application");

	while (Ikigai::Platform::PumpMessages(&state)) {

	}
	Ikigai::Platform::Shutdown(&state);
}