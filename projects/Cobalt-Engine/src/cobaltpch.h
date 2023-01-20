#pragma once

#include "Application/Core.h"
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
#include <chrono>
#include <thread>

#ifdef COBALT_PLATFORM_WINDOWS
#include <Windows.h>
#include <Windowsx.h>
#endif