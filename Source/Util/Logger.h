#pragma once

#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <string>

namespace EarthRenderer
{
    enum class LogLevel
    {
        ALL = -1,
        TRACE = 0,
        DEBUG = 1,
        INFO = 2,
        WARNING = 3,
        FATAL = 4,
        NONE = 5
    };

    class Logger
    {
      public:
        Logger() = delete;

        static void Log(LogLevel level, const std::string& message);
        static void SetLogLevel(LogLevel logLevel);
        static bool isLogEnabledFor(LogLevel level);
        static LogLevel GetLogLevel();
        static std::string GetTimeString();

        inline static std::string GetLogLevelString(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::ALL:
                    return "ALL";
                case LogLevel::NONE:
                    return "NONE";
                case LogLevel::TRACE:
                    return "TRACE";
                case LogLevel::DEBUG:
                    return "DEBUG";
                case LogLevel::INFO:
                    return "INFO";
                case LogLevel::WARNING:
                    return "WARN";
                case LogLevel::FATAL:
                    return "FATAL";
                default:
                    return "N/A";
            }
        }

      private:
        static LogLevel mLogLevel;
        static std::atomic_uint32_t mLastThreadId;
    };
}

#define LOG_PRIVATE(LEVEL, FORMAT, ...)                                          \
    do                                                                           \
    {                                                                            \
        if (EarthRenderer::Logger::isLogEnabledFor(LEVEL))                       \
        {                                                                        \
            EarthRenderer::Logger::Log(LEVEL, std::format(FORMAT, __VA_ARGS__)); \
        }                                                                        \
    } while (false)

#define LOG_TRACE(FORMAT, ...) LOG_PRIVATE(EarthRenderer::LogLevel::TRACE, FORMAT, __VA_ARGS__)
#define LOG_DEBUG(FORMAT, ...) LOG_PRIVATE(EarthRenderer::LogLevel::DEBUG, FORMAT, __VA_ARGS__)
#define LOG_INFO(FORMAT, ...)  LOG_PRIVATE(EarthRenderer::LogLevel::INFO, FORMAT, __VA_ARGS__)
#define LOG_WARN(FORMAT, ...)  LOG_PRIVATE(EarthRenderer::LogLevel::WARNING, FORMAT, __VA_ARGS__)
#define LOG_FATAL(FORMAT, ...) LOG_PRIVATE(EarthRenderer::LogLevel::FATAL, FORMAT, __VA_ARGS__)

#define EARTH_ASSERT(EXPRESSION, FORMAT, ...) \
    do                                        \
    {                                         \
        if ((EXPRESSION) == false)            \
        {                                     \
            LOG_FATAL(FORMAT, __VA_ARGS__);   \
            std::exit(EXIT_FAILURE);          \
        }                                     \
    } while (false)
