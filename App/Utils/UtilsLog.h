#include <format>
#include <iostream>
class UtilsLog {
public:

    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERR
    };

    template<typename... Args>
    static void info(std::format_string<Args...> fmt, Args&&... args)
    {
        print(LogLevel::INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        print(LogLevel::DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void warning(std::format_string<Args...> fmt, Args&&... args)
    {
        print(LogLevel::WARN, std::format(fmt, std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void error(std::format_string<Args...> fmt, Args&&... args)
    {
        print(LogLevel::ERR, std::format(fmt, std::forward<Args>(args)...));
    }

private:
    static void print(LogLevel level, const std::string& msg)
    {
        std::cout << levelToColor(level)
                << "[" << levelToString(level) << "] "
                << msg
                << "\033[0m"
                << std::endl;
    }

    static const char* levelToColor(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "\033[36m";
            case LogLevel::INFO:  return "\033[37m";
            case LogLevel::WARN:  return "\033[33m";
            case LogLevel::ERR:   return "\033[31m";
            default:              return "\033[0m";
        }
    }

    static const char* levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARNING";
            case LogLevel::ERR:   return "ERROR";
            default:              return "LOG";
        }
    }

    // | Màu        | Code       |
    // | ---------- | ---------- |
    // | Đen        | `\033[30m` |
    // | Đỏ         | `\033[31m` |
    // | Xanh lá    | `\033[32m` |
    // | Vàng       | `\033[33m` |
    // | Xanh dương | `\033[34m` |
    // | Tím        | `\033[35m` |
    // | Cyan       | `\033[36m` |
    // | Trắng      | `\033[37m` |

};
