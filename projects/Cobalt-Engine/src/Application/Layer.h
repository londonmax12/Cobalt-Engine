#pragma once

#include "Event/Event.h"
#include "Time/DeltaTime.h"

namespace Cobalt {
	class Layer {
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(DeltaTime deltaTime) {};
		virtual void OnEvent(Event& e) {};

		const char* GetName() { return m_LayerName; }
	private:
		const char* m_LayerName;
	};
}