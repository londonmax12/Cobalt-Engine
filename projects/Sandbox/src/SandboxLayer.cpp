#include "SandboxLayer.h"

#include "Logging/Logging.h"
#include "Event/Event.h"

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(Cobalt::DeltaTime deltaTime)
{
}

void SandboxLayer::OnEvent(Cobalt::Event& e)
{
	COBALT_TRACE_CLIENT("{}", e.ToString());
}
