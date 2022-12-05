#include "ikipch.h"
#include "Application.h"

Ikigai::ApplicationConfig Ikigai::ApplicationConfig::createConfig()
{
	ApplicationConfig conf = ApplicationConfig();
	conf.PositionX = 0;
	conf.PositionY = 0;
	conf.Width = 1600;
	conf.Height = 900;
	conf.ApplicationName = "Ikigai Application";
	return conf;
}

bool Ikigai::Application::Init(ApplicationConfig config)
{
	if (m_Instance) {
		IKIGAI_ASSERT_MSG(false, "Ikigai application already exists");
		return false;
	}

	Ikigai::Logging::Init();
	IKIGAI_INFO("Initialized logging system");

	Ikigai::Platform::Init();
	IKIGAI_INFO("Initialized platform layer");

	Ikigai::Platform::PlatformState state{};
	bool success = Ikigai::Platform::Startup(&state, config.ApplicationName, config.PositionX, config.PositionY, config.Width, config.Height);
	IKIGAI_ASSERT_MSG(success, "Failed to start application!");
	if (!success)
		return false;

	IKIGAI_INFO("Created platform application");

	m_State = &state;

	m_Running = true;

	m_Instance = this;
	return true;
}

void Ikigai::Application::Run()
{
	while (m_Running) {
		if (!Ikigai::Platform::PumpMessages(m_State))
			m_Running = false;		
	}
	Ikigai::Platform::Shutdown(m_State);
}
