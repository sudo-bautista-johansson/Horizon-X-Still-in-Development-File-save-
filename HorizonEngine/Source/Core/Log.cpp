#include "Horizon/Core/Log.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdarg>
#include <cstdio>

namespace Horizon {

    std::vector<LogMessage> Log::s_Messages;
    std::mutex Log::s_LogMutex;

    void Log::Init()
    {
        // Simple initialization, could configure colors/output streams here
        std::lock_guard<std::mutex> lock(s_LogMutex);
        s_Messages.reserve(1000);
    }

    const std::vector<LogMessage>& Log::GetMessages()
    {
        return s_Messages;
    }

    void Log::ClearMessages()
    {
        std::lock_guard<std::mutex> lock(s_LogMutex);
        s_Messages.clear();
    }

    std::mutex& Log::GetMutex()
    {
        return s_LogMutex;
    }

    void Log::LogFormatted(bool isCore, LogLevel level, const char* fmt, ...)
    {
        std::lock_guard<std::mutex> lock(s_LogMutex);

        // Get current time
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
        std::string timeStr = ss.str();

        // Format message using va_list
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        std::string messageStr = (isCore ? "[CORE] " : "[APP] ") + std::string(buffer);

        // Add to our messages array (for Console UI)
        s_Messages.push_back({ level, messageStr, timeStr });

        // Console output (with color if possible, standard coloring depending on platform)
        const char* colorCode = "\033[0m"; // reset
        const char* levelStr = "";
        switch (level)
        {
            case LogLevel::Trace: colorCode = "\033[37m"; levelStr = "TRACE"; break; // White
            case LogLevel::Info:  colorCode = "\033[32m"; levelStr = "INFO";  break; // Green
            case LogLevel::Warn:  colorCode = "\033[33m"; levelStr = "WARN";  break; // Yellow
            case LogLevel::Error: colorCode = "\033[31m"; levelStr = "ERROR"; break; // Red
        }

        std::cout << colorCode << "[" << timeStr << "] " << levelStr << ": " << messageStr << "\033[0m\n";
    }

} // namespace Horizon
