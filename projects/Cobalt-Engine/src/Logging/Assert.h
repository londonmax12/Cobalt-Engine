#pragma once

// Comment line to disable assertions
#define COBALT_ASSERTIONS_ENABLED

namespace Cobalt {
	void ReportAssertion(const char* expression, const char* message, const char* file, int line);
}

#ifdef COBALT_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define CobaltDebugBreak() __debugbreak()
#else
#define CobaltDebugBreak() __builtin_trap()
#endif

#define COBALT_ASSERT(expr) {												\
	if(expr) {																\
	} else {																\
		Cobalt::ReportAssertion(#expr, "", __FILE__, __LINE__);		\
		CobaltDebugBreak();													\
	}																		\
}
#define COBALT_ASSERT_MSG(expr, message) {									\
	if(expr) {																\
	} else {																\
		Cobalt::ReportAssertion(#expr, message, __FILE__, __LINE__);		\
		CobaltDebugBreak();													\
	}																		\
}

#else
#define COBALT_ASSERT(expr)
#define COBALT_ASSERT_MSG(expr, message)
#endif


#ifdef COBALT_DEBUG_MODE
#define COBALT_DEBUG_ASSERT(expr) COBALT_ASSERT(expr)
#define COBALT_DEBUG_ASSERT_MSG(expr, message) COBALT_ASSERT_MSG(expr,message)
#else
#define COBALT_DEBUG_ASSERT(expr)
#define COBALT_DEBUG_ASSERT_MSG(expr, message)
#endif