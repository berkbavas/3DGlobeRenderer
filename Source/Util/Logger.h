#pragma once

#include <assert.h>
#include <atomic>
#include <format>
#include <map>
#include <string>

#include <QDebug>

namespace GlobeRenderer
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

        static void Log(LogLevel Level, const std::string& LogMessage);
        static void SetLogLevel(LogLevel LogLevel);
        static bool isLogEnabledFor(LogLevel Level);
        static LogLevel GetLogLevel();
        static std::string GetTimeString();

        static void QtMessageOutputCallback(QtMsgType Type, const QMessageLogContext& Context, const QString& LogMessage);

        inline static std::string GetLogLevelString(LogLevel LogLevel)
        {
            switch (LogLevel)
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

#define LOG_PRIVATE(LEVEL, FORMAT, ...) \
    do \
    { \
        if (GlobeRenderer::Logger::isLogEnabledFor(LEVEL)) \
        { \
            GlobeRenderer::Logger::Log(LEVEL, std::format(FORMAT, __VA_ARGS__)); \
        } \
    } while (false)

#define LOG_TRACE(FORMAT, ...) LOG_PRIVATE(GlobeRenderer::LogLevel::TRACE, FORMAT, __VA_ARGS__)
#define LOG_DEBUG(FORMAT, ...) LOG_PRIVATE(GlobeRenderer::LogLevel::DEBUG, FORMAT, __VA_ARGS__)
#define LOG_INFO(FORMAT, ...) LOG_PRIVATE(GlobeRenderer::LogLevel::INFO, FORMAT, __VA_ARGS__)
#define LOG_WARN(FORMAT, ...) LOG_PRIVATE(GlobeRenderer::LogLevel::WARNING, FORMAT, __VA_ARGS__)
#define LOG_FATAL(FORMAT, ...) LOG_PRIVATE(GlobeRenderer::LogLevel::FATAL, FORMAT, __VA_ARGS__)

#define GR_ASSERT(EXPRESSION) assert(EXPRESSION)

#define GR_EXIT_FAILURE(FORMAT, ...) \
    do \
    { \
        LOG_FATAL(FORMAT, __VA_ARGS__); \
        std::exit(EXIT_FAILURE); \
    } while (false)

#define GR_EXIT_FAILURE_IF(CONDITION, FORMAT, ...) \
    do \
    { \
        if (CONDITION) \
        { \
            LOG_FATAL(FORMAT, __VA_ARGS__); \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false)