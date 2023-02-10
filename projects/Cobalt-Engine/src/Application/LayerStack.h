#pragma once

#include "Application/Layer.h"
#include "Application/Core.h"

#include <vector>

namespace Cobalt {
	class LayerStack {
	public:
		LayerStack();

		void OnEvent(Event& e);
		void OnUpdate(DeltaTime deltaTime);

		void PushLayer(Ref<Layer> layer);
		void PushOverlay(Ref<Layer> overlay);
		void PopLayer(Ref<Layer> layer);
		void PopOverlay(Ref<Layer> overlay);

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Ref<Layer>> m_Layers;
		int m_LayerInsert;
	};
}