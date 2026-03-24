#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <format>
#include <iostream>
#include <mutex>
#include <string>

class UtilsLog {
public:

    // LOG LEVELS
    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERR
    };

    // LOG FUNCTION
    template<typename... Args>
    static void log(LogLevel level,
                    const char* file,
                    int line,
                    const char* func,
                    std::format_string<Args...> fmt,
                    Args&&... args)
    {
        write(level, file, line, func, std::format(fmt, std::forward<Args>(args)...));
    }

private:
    // MUTEX
    static inline std::mutex mtx;

    // WRITE FUNCTION
    static void write(LogLevel level,
                      const char* file,
                      int line,
                      const char* func,
                      const std::string& msg)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << levelToColor(level)
                  << "[" << levelToString(level) << "] "
                  << file << ":" << line
                  << " (" << func << ") "
                  << msg
                  << "\033[0m"
                  << std::endl;
    }

    // COLOR AND STRING CONVERSION
    static const char* levelToColor(LogLevel level)
    {
        switch (level) {
            case LogLevel::DEBUG: return "\033[36m"; // Cyan
            case LogLevel::INFO:  return "\033[37m"; // White
            case LogLevel::WARN:  return "\033[33m"; // Yellow
            case LogLevel::ERR:   return "\033[31m"; // Red
            default:              return "\033[0m";
        }
    }

    // LOG LEVEL TO STRING
    static const char* levelToString(LogLevel level)
    {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERR:   return "ERROR";
            default:              return "LOG";
        }
    }
};

// MACRO WRAPPER
#define LOGD(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::ERR,   __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif // UTILS_LOG_H
