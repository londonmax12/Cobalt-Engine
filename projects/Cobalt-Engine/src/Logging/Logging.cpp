#include "cobaltpch.h"

void Cobalt::Logging::Init()
{
	s_ClientLogger = std::make_shared<Logger>(Logger("CLIENT"));
	s_CoreLogger = std::make_shared<Logger>(Logger("COBALT"));
}
