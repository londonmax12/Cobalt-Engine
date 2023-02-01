#pragma once

#include "Event/Event.h"
#include "Event/SurfaceEvents.h"

#include "Platforms/Platform.h"

namespace Cobalt {
	class RendererBackend
	{
	public:
		virtual bool Init(const char* applicationName, Platform::PlatformState* platformState, int width, int height) = 0;
		virtual void Shutdown() = 0;
		virtual void Resized(int width, int height) = 0;

		virtual bool BeginFrame(DeltaTime dt) = 0;
		virtual bool EndFrame(DeltaTime dt) = 0;

	private:
		Platform::PlatformState* m_PlatformState = nullptr;
	};

	struct RenderCommand
	{
		DeltaTime deltaTime;
	};
}