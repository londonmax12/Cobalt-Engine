#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define COBALT_PLATFORM_WINDOWS
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define COBALT_PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_MACCATALYST
#define COBALT_PLATFORM_MAC_CATALYST
#elif TARGET_OS_IPHONE
#define COBALT_PLATFORM_IOS
#elif TARGET_OS_MAC
#define COBALT_PLATFORM_MAC
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
#define COBALT_PLATFORM_LINUX
#elif __unix__
#define COBALT_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define COBALT_PLATFORM_POSIX
#else
#   error "Unknown compiler"
#endif