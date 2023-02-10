#pragma once

#include "Time/DeltaTime.h"
#include "Application/Layer.h"
#include "Event/Event.h"

class SandboxLayer : public Cobalt::Layer{
public:
	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Cobalt::DeltaTime deltaTime) override;
	void OnEvent(Cobalt::Event& e) override;
};