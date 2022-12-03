#include "ikipch.h"

void Ikigai::Logging::Init()
{
	s_ClientLogger = std::make_shared<Logger>(Logger("CLIENT"));
	s_CoreLogger = std::make_shared<Logger>(Logger("CORE"));
}
