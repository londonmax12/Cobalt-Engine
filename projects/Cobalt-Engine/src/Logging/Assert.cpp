#include "cobaltpch.h"
#include "Assert.h"

void Cobalt::ReportAssertion(const char* expression, const char* message, const char* file, int line)
{
	if (message == "")
		Cobalt::Logging::GetCoreLogger()->Log(Cobalt::Logger::LOG_SEVERITY_FATAL, "Assertion Failed ({}) at line {} in {}", expression, line, file);
	else
		Cobalt::Logging::GetCoreLogger()->Log(Cobalt::Logger::LOG_SEVERITY_FATAL, "Assertion Failed ({}) at line {} in {}: {}", expression, line, file, message);
}