#include "ikipch.h"
#include "Application.h"
#include "Event/Event.h"

bool AppCloseCallback(Ikigai::Ref<Ikigai::Event> ev)
{
	Ikigai::Application::GetInstance()->m_Running = false;
	return true;
}

Ikigai::ApplicationConfig Ikigai::ApplicationConfig::createConfig()
{
	ApplicationConfig conf = ApplicationConfig();
	conf.PositionX = 40;
	conf.PositionY = 40;
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

	Ikigai::Platform::PlatformState state{};
	bool success = Ikigai::Platform::Startup(&state, config.ApplicationName, config.PositionX, config.PositionY, config.Width, config.Height);
	IKIGAI_ASSERT_MSG(success, "Failed to start application!");
	if (!success)
		return false;

	IKIGAI_INFO("Created platform application");

	EventSystem::Register(EventListener(EVENTTYPE_APPLICATION_CLOSE, &AppCloseCallback));
	IKIGAI_INFO("Created application event listeners");

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
		EventSystem::PollEvents();
	}
	Ikigai::Platform::Shutdown(m_State);


	EventSystem::Shutdown();
}