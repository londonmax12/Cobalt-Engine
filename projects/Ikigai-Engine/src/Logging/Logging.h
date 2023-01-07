#pragma once
#include <string>
#include <format>

#include "Math/Vector.h"
#include "Math/Vertex.h"
#include "Platforms/Platform.h"
#include "Time/DeltaTime.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

#if _DEBUG
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#else
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

#define IKIGAI_FATAL(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_FATAL, message, ##__VA_ARGS__)
#define IKIGAI_ERROR(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_ERROR, message, ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
#define IKIGAI_WARN(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_WARN, message, ##__VA_ARGS__)
#else
#define IKIGAI_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define IKIGAI_INFO(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_INFO, message, ##__VA_ARGS__)
#else
#define IKIGAI_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define IKIGAI_DEBUG(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_DEBUG, message, ##__VA_ARGS__)
#else
#define IKIGAI_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define IKIGAI_TRACE(message, ...) Ikigai::Logging::GetCoreLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_TRACE, message, ##__VA_ARGS__)
#else
#define IKIGAI_TRACE(message, ...)
#endif

#define IKIGAI_FATAL_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_FATAL, message, ##__VA_ARGS__)
#define IKIGAI_ERROR_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_ERROR, message, ##__VA_ARGS__)

#if LOG_WARN_ENABLED == 1
#define IKIGAI_WARN_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_WARN, message, ##__VA_ARGS__)
#else
#define IKIGAI_WARN_CLIENT(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define IKIGAI_INFO_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_INFO, message, ##__VA_ARGS__)
#else
#define IKIGAI_INFO_CLIENT(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define IKIGAI_DEBUG_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_DEBUG, message, ##__VA_ARGS__)
#else
#define IKIGAI_DEBUG_CLIENT(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define IKIGAI_TRACE_CLIENT(message, ...) Ikigai::Logging::GetClientLogger()->Log(Ikigai::Logger::LogSeverityLevel::LOG_SEVERITY_TRACE, message, ##__VA_ARGS__)
#else
#define IKIGAI_TRACE_CLIENT(message, ...)
#endif

template <>
struct std::formatter<Ikigai::Vector2> : std::formatter<std::string> {
	auto format(Ikigai::Vector2 v, format_context& ctx) {
		return formatter<string>::format(
			std::format("[{}, {}]", v.x, v.y), ctx);
	}
};
template <>
struct std::formatter<Ikigai::Vector3> : std::formatter<std::string> {
	auto format(Ikigai::Vector3 v, format_context& ctx) {
		return formatter<string>::format(
			std::format("[{}, {}, {}]", v.x, v.y, v.z), ctx);
	}
};
template <>
struct std::formatter<Ikigai::Vector4> : std::formatter<std::string> {
	auto format(Ikigai::Vector4 v, format_context& ctx) {
		return formatter<string>::format(
			std::format("[{}, {}, {}, {}]", v.x, v.y, v.z, v.w), ctx);
	}
};
template <>
struct std::formatter<Ikigai::Vertex3D> : std::formatter<std::string> {
	auto format(Ikigai::Vertex3D v, format_context& ctx) {
		return formatter<string>::format(
			std::format("{}", v.Position), ctx);
	}
};
template <>
struct std::formatter<Ikigai::Timer> : std::formatter<std::string> {
	auto format(Ikigai::Timer t, format_context& ctx) {
		return formatter<string>::format(
			std::format("{}", t.ElapsedMilliseconds()), ctx);
	}
};

template <>
struct std::formatter<Ikigai::DeltaTime> : std::formatter<std::string> {
	auto format(Ikigai::DeltaTime t, format_context& ctx) {
		return formatter<string>::format(
			std::format("{}", t.ElapsedMilliseconds()), ctx);
	}
};


namespace Ikigai {
	class Logger {
	public:
		Logger() = default;
		Logger(const char* loggerName) : name(loggerName) {}

		enum LogSeverityLevel {
			LOG_SEVERITY_TRACE = 0,
			LOG_SEVERITY_DEBUG = 1,
			LOG_SEVERITY_INFO = 2,
			LOG_SEVERITY_WARN = 3,
			LOG_SEVERITY_ERROR = 4,
			LOG_SEVERITY_FATAL = 5,
		};


		template<typename... Args>
		void Log(LogSeverityLevel level, std::string msg, const Args&... args) {
			bool isError = (level == Logger::LOG_SEVERITY_ERROR || level == Logger::LOG_SEVERITY_FATAL);
			const char* str[6] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };
			const int msgSize = 5000;
			char out[msgSize];
			memset(out, 0, sizeof(out));

			char outMsg[msgSize];
			sprintf_s(outMsg, msgSize, "[%s]\t%s: %s\n", str[(int)level], name, std::vformat(msg, std::make_format_args(args...)).c_str());

			if (isError) {
				Platform::ConsoleWriteError(outMsg, level);
			}
			else {
				Platform::ConsoleWrite(outMsg, level);
			}
		};
	private:
		const char* name = "UnnamedLogger";
	};

	class Logging
	{
	public:
		static void Init();

		static std::shared_ptr<Logger>& GetCoreLogger() { return s_CoreLogger; };
		static std::shared_ptr<Logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		inline static std::shared_ptr<Logger> s_CoreLogger;
		inline static std::shared_ptr<Logger> s_ClientLogger;
	};
}