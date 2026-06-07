#pragma once

#include "Horizon/Core/Base.h"
#include <string>
#include <sstream>
#include <vector>
#include <mutex>

namespace Horizon {

    enum class LogLevel
    {
        Trace = 0,
        Info,
        Warn,
        Error
    };

    struct LogMessage
    {
        LogLevel Level;
        std::string Message;
        std::string Time;
    };

    class Log
    {
    public:
        static void Init();
        
        template<typename... Args>
        static void CoreLog(LogLevel level, const std::string& fmt, Args&&... args)
        {
            LogFormatted(true, level, fmt.c_str(), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void ClientLog(LogLevel level, const std::string& fmt, Args&&... args)
        {
            LogFormatted(false, level, fmt.c_str(), std::forward<Args>(args)...);
        }

        static const std::vector<LogMessage>& GetMessages();
        static void ClearMessages();
        static std::mutex& GetMutex();

    private:
        static void LogFormatted(bool isCore, LogLevel level, const char* fmt, ...);
        static std::vector<LogMessage> s_Messages;
        static std::mutex s_LogMutex;
    };

} // namespace Horizon

// Core log macros
#define HZ_CORE_TRACE(...) ::Horizon::Log::CoreLog(::Horizon::LogLevel::Trace, __VA_ARGS__)
#define HZ_CORE_INFO(...)  ::Horizon::Log::CoreLog(::Horizon::LogLevel::Info, __VA_ARGS__)
#define HZ_CORE_WARN(...)  ::Horizon::Log::CoreLog(::Horizon::LogLevel::Warn, __VA_ARGS__)
#define HZ_CORE_ERROR(...) ::Horizon::Log::CoreLog(::Horizon::LogLevel::Error, __VA_ARGS__)

// Client log macros
#define HZ_TRACE(...)      ::Horizon::Log::ClientLog(::Horizon::LogLevel::Trace, __VA_ARGS__)
#define HZ_INFO(...)       ::Horizon::Log::ClientLog(::Horizon::LogLevel::Info, __VA_ARGS__)
#define HZ_WARN(...)       ::Horizon::Log::ClientLog(::Horizon::LogLevel::Warn, __VA_ARGS__)
#define HZ_ERROR(...)      ::Horizon::Log::ClientLog(::Horizon::LogLevel::Error, __VA_ARGS__)
