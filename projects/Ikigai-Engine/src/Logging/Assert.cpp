#include "ikipch.h"
#include "Assert.h"

void Ikigai::ReportAssertion(const char* expression, const char* message, const char* file, int line)
{
	if(message == "")
		Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LOG_SEVERITY_FATAL, "Assertion Failed ({}) at line {} in {}", expression, line, file);
	else
		Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LOG_SEVERITY_FATAL, "Assertion Failed ({}) at line {} in {}: {}", expression, line, file, message);
}