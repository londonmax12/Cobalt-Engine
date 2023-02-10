#include "cobaltpch.h"

#include "LayerStack.h"

Cobalt::LayerStack::LayerStack()
{
	m_LayerInsert = 0;
}

void Cobalt::LayerStack::OnEvent(Event& e)
{
	for (auto it = m_Layers.end(); it != m_Layers.begin();) {
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

void Cobalt::LayerStack::OnUpdate(DeltaTime deltaTime)
{
	for (auto& layer : m_Layers)
		layer->OnUpdate(deltaTime);
}

void Cobalt::LayerStack::PushLayer(Ref<Layer> layer)
{
	m_LayerInsert = (int)(m_Layers.emplace(m_Layers.begin() + m_LayerInsert, layer) - m_Layers.begin());
	m_LayerInsert++;
}

void Cobalt::LayerStack::PushOverlay(Ref<Layer> overlay)
{
	m_Layers.emplace_back(overlay);
}

void Cobalt::LayerStack::PopLayer(Ref<Layer> layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

void Cobalt::LayerStack::PopOverlay(Ref<Layer> overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}
