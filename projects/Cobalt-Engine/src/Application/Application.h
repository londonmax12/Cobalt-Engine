#pragma once
#include "Platforms/Platform.h"
#include "Core.h"
#include "Event/ApplicationEvents.h"
#include "Time/DeltaTime.h"
#include "Application/Layer.h"
#include "Application/LayerStack.h"

namespace Cobalt {
	struct ApplicationConfig {
		ApplicationConfig() = default;
		ApplicationConfig(const char* name, Vector2 size = {1600,900}, Vector2 position = Vector2{0}) : 
			ApplicationName(name), Width((int)size.x), Height((int)size.y), PositionX((int)position.x), PositionY((int)position.y) {};

		int PositionY = 0;
		int PositionX = 0;

		int Width = 1600;
		int Height = 900;

		const char* ApplicationName = "Snow Application";
		
		bool LimitFrameRate = false;
		int FrameLimit = 60;
	};

	class Application {
	public:
		Application(ApplicationConfig config) { Init(config); }
		bool Init(ApplicationConfig config);

		void Run();
		static Application* GetInstance() { return m_Instance; }

		bool IsInitialized() { return m_Instance != nullptr; }

		void OnEvent(Event& e);
		bool OnAppClose(ApplicationCloseEvent& ev);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);

		void PopLayer(Ref<Layer> layer);
		void PopOverlay(Ref<Layer> overlay);
	private:
		inline static Application* m_Instance = nullptr;

		bool m_Running = false;
		bool m_Suspended = false;

		Ref<Platform::PlatformState> m_State;

		int m_Width = 0;
		int m_Height = 0;

		bool m_LimitFrames = false;
		int m_FrameLimit = 60;

		LayerStack m_LayerStack;
	};

	// Defined by client not engine
	Application* CreateApplication();
}