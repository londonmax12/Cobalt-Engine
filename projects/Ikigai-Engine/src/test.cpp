#include "ikipch.h"
#include "test.h"

void Test::PrintMessage() {
#ifdef IKIGAI_DEBUG_MODE
	std::cout << "Hello World (Debug)\n";
#elif IKIGAI_DISTRIBUTION_MODE
	std::cout << "Hello World (Distribution)\n";
#elif IKIGAI_RELEASE_MODE
	std::cout << "Hello World (Release)\n";
#endif
}