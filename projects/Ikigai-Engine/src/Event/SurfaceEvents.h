#pragma once
#include "Event.h"

namespace Ikigai {
	class SurfaceResizeEvent : public Event {
	public:
		SurfaceResizeEvent(int width, int height) : Event(EVENTTYPE_SURFACE_RESIZE), Width(width), Height(height) {};
		int Width;
		int Height;
	};
}