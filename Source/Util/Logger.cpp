
#include "Logger.h"

void EarthRenderer::Logger::Log(LogLevel level, const std::string& message)
{
    thread_local const auto TL_THREAD_ID = mLastThreadId.fetch_add(1);

    const auto log = std::format("{:.12} [{:}] [ {:5} ] {:}", GetTimeString(), TL_THREAD_ID, GetLogLevelString(level), message);

    switch (level)
    {
        case LogLevel::TRACE:
        case LogLevel::DEBUG:

        case LogLevel::INFO:
            std::cout << log << std::endl;
            break;
        case LogLevel::WARNING:
        case LogLevel::FATAL:
            std::cerr << log << std::endl;
            break;
        default:
            break;
    }
}

void EarthRenderer::Logger::SetLogLevel(LogLevel logLevel)
{
    mLogLevel = logLevel;
}

EarthRenderer::LogLevel EarthRenderer::Logger::GetLogLevel()
{
    return mLogLevel;
}

std::string EarthRenderer::Logger::GetTimeString()
{
    const auto zone = std::chrono::current_zone();
    const auto now = std::chrono::system_clock::now();
    const auto local = std::chrono::zoned_time(zone, now);

    return std::format("{:%T}", local);
}

bool EarthRenderer::Logger::isLogEnabledFor(LogLevel level)
{
    return mLogLevel <= level;
}

EarthRenderer::LogLevel EarthRenderer::Logger::mLogLevel = EarthRenderer::LogLevel::ALL;

std::atomic_uint32_t EarthRenderer::Logger::mLastThreadId = 0;