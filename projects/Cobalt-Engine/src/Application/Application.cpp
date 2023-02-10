#include "cobaltpch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input/Input.h"
#include "Debug/Debug.h"
#include "Renderer/Renderer.h"

bool Cobalt::Application::Init(ApplicationConfig config)
{
	if (m_Instance) {
		COBALT_ASSERT_MSG(false, "Cobalt application already exists");
		return false;
	}

	m_Width = config.Width;
	m_Height = config.Height;
	m_FrameLimit = config.FrameLimit;
	m_LimitFrames = config.LimitFrameRate;
	m_State = CreateRef<Platform::PlatformState>();

	{
		bool success = Cobalt::Platform::Startup(m_State, config.ApplicationName, config.PositionX, config.PositionY, m_Width, m_Height);
		COBALT_ASSERT_MSG(success, "Failed to start application!");
		if (!success)
			return false;
	}
	COBALT_INFO("Created platform application");

	{
		RendererConfig conf = RendererConfig(RENDERER_BACKEND_VULKAN);
		conf.State = m_State;

		bool success = Renderer::Init(conf);
		COBALT_ASSERT_MSG(success, "Failed to initialize renderer!");
		if (!success)
			return false;
	}
	COBALT_INFO("Initialized renderer");

	Cobalt::EventSystem::GetInstance()->AddCallback(COBALT_BIND_EVENT_FN(OnEvent));

	m_Running = true;

	m_Instance = this;

	return true;
}

void Cobalt::Application::Run()
{
	Timer timer{};
	timer.Start();

	double lastTime = timer.Elapsed();
	int frameCount = 0;
	float targetFrames = 1.0f / m_FrameLimit;

	while (m_Running) {
		double currTime = timer.Elapsed();
		double frameStart = Platform::GetAbsTime();

		if (!Cobalt::Platform::PumpMessages(m_State))
			m_Running = false;

		double delta = currTime - lastTime;
		lastTime = currTime;

		m_LayerStack.OnUpdate(DeltaTime((float)delta));

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

	Renderer::Shutdown();
	Platform::Shutdown(m_State);
}

void Cobalt::Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<ApplicationCloseEvent>(COBALT_BIND_EVENT_FN(OnAppClose));

	Renderer::GetInstance()->OnEvent(e);
	m_LayerStack.OnEvent(e);
}

bool Cobalt::Application::OnAppClose(ApplicationCloseEvent& ev)
{
	m_Running = false;
	return true;
}

void Cobalt::Application::PushLayer(Ref<Layer> layer)
{
	m_LayerStack.PushLayer(layer);
}

void Cobalt::Application::PushOverlay(Ref<Layer> overlay)
{
	m_LayerStack.PushOverlay(overlay);
}

void Cobalt::Application::PopLayer(Ref<Layer> layer)
{
	m_LayerStack.PopLayer(layer);
}

void Cobalt::Application::PopOverlay(Ref<Layer> overlay)
{
	m_LayerStack.PopOverlay(overlay);
}
