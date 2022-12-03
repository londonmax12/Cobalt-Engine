#pragma once

#include "Logging/Logging.h"
#include "Logging/Assert.h"
#include "Platforms/PlatformDetection.h"
#include "Math/Math.h"

#include <format>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <algorithm>
#include <cstdarg>
#include <sys/stat.h>

#ifdef IKIGAI_PLATFORM_WINDOWS
#include <Windows.h>
#endif