#pragma once

// Comment line to disable assertions
#define IKIGAI_ASSERTIONS_ENABLED

namespace Ikigai {
	void ReportAssertion(const char* expression, const char* message, const char* file, int line);
}

#ifdef IKIGAI_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define IkigaiDebugBreak() __debugbreak()
#else
#define IkigaiDebugBreak() __builtin_trap()
#endif

#define IKIGAI_ASSERT(expr) {												\
	if(expr) {																\
	} else {																\
		Ikigai::ReportAssertion(#expr, "", __FILE__, __LINE__);		\
		IkigaiDebugBreak();													\
	}																		\
}
#define IKIGAI_ASSERT_MSG(expr, message) {									\
	if(expr) {																\
	} else {																\
		Ikigai::ReportAssertion(#expr, message, __FILE__, __LINE__);		\
		IkigaiDebugBreak();													\
	}																		\
}

#else
#define IKIGAI_ASSERT(expr)
#define IKIGAI_ASSERT_MSG(expr, message)
#endif


#ifdef IKIGAI_DEBUG_MODE
#define IKIGAI_DEBUG_ASSERT(expr) IKIGAI_ASSERT(expr)
#define IKIGAI_DEBUG_ASSERT_MSG(expr, message) IKIGAI_ASSERT_MSG(expr,message)
#else
#define IKIGAI_DEBUG_ASSERT(expr)
#define IKIGAI_DEBUG_ASSERT_MSG(expr, message)
#endif