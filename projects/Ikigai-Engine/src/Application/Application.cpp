#include "ikipch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input/Input.h"
#include "Debug/Debug.h"
#include "Renderer/Renderer.h"

bool Ikigai::Application::Init(ApplicationConfig config)
{
	if (m_Instance) {
		IKIGAI_ASSERT_MSG(false, "Ikigai application already exists");
		return false;
	}

	m_Width = config.Width;
	m_Height = config.Height;
	m_FrameLimit = config.FrameLimit;
	m_LimitFrames = config.LimitFrameRate;

	Ikigai::Platform::PlatformState state{};
	{
		bool success = Ikigai::Platform::Startup(&state, config.ApplicationName, config.PositionX, config.PositionY, m_Width, m_Height);
		IKIGAI_ASSERT_MSG(success, "Failed to start application!");
		if (!success)
			return false;
	}
	IKIGAI_INFO("Created platform application");

	{
		bool success = Renderer::Init(RendererConfig(RENDERER_BACKEND_VULKAN));
		IKIGAI_ASSERT_MSG(success, "Failed to initialize renderer!");
		if (!success)
			return false;
	}
	IKIGAI_INFO("Initialized renderer");

	Ikigai::EventSystem::GetInstance()->AddCallback(IKIGAI_BIND_EVENT_FN(OnEvent));

	m_State = &state;

	m_Running = true;

	m_Instance = this;

	return true;
}

void Ikigai::Application::Run()
{
	Timer timer{};
	timer.Start();

	double lastTime = timer.Elapsed();
	int frameCount = 0;
	float targetFrames = 1.0f / m_FrameLimit;

	while (m_Running) {
		double currTime = timer.Elapsed();
		double frameStart = Platform::GetAbsTime();

		if (!Ikigai::Platform::PumpMessages(m_State))
			m_Running = false;

		double delta = currTime - lastTime;
		lastTime = currTime;
		OnUpdate(DeltaTime((float)delta));

		double frameEnd = Platform::GetAbsTime();
		double frameTime = frameEnd - frameStart;
		double deltaSeconds = targetFrames - frameTime;


		if (deltaSeconds > 0) {
			unsigned int remainingMs = (unsigned int)(deltaSeconds * 1000);

			if (m_LimitFrames && remainingMs > 0) {
				Platform::Sleep(remainingMs);
			}

			frameCount++;
		}

		static int lastFrameRecord = 0;
		int second = (int)std::floor(timer.Elapsed());
		if (second > lastFrameRecord)
		{
			Debug::SetFrameRate(frameCount);
			frameCount = 0;
		}
		lastFrameRecord = second;
	}

	Ikigai::Platform::Shutdown(m_State);
}

void Ikigai::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<ApplicationCloseEvent>(IKIGAI_BIND_EVENT_FN(OnAppClose));

	Renderer::GetInstance()->OnEvent(e);
}

bool Ikigai::Application::OnAppClose(ApplicationCloseEvent& ev)
{
	m_Running = false;
	return true;
}
