#pragma once
#include "Platforms/Platform.h"
#include "Core.h"
#include "Event/ApplicationEvents.h"
#include "Time/DeltaTime.h"

namespace Ikigai {
	struct ApplicationConfig {
		int PositionY;
		int PositionX;

		int Width;
		int Height;

		const char* ApplicationName;

		static ApplicationConfig createConfig();
	};

	class Application {
	public:
		bool Init(ApplicationConfig config);
		void Run();
		static Application* GetInstance() { return m_Instance; }

		virtual void OnUpdate(DeltaTime dt) = 0;
	private:
		inline static Application* m_Instance = nullptr;

		static bool AppCloseCallback(Ikigai::Ref<Ikigai::Event> ev);

		bool m_Running = false;
		bool m_Suspended = false;

		Platform::PlatformState* m_State = nullptr;

		DeltaTime deltaTime;

		int m_Width = 0;
		int m_Height = 0;
	};

	// Defined by client not engine
	Application* CreateApplication();
}