#include "utils/logger.hpp"

#include <fstream>

namespace vrt {
namespace utils {

std::unique_ptr<std::ofstream> Logger::fileStream_ = nullptr;
std::ostream* Logger::output_ = &std::cout;
LogLevel Logger::currentLogLevel_ = LogLevel::INFO;

void Logger::setOutput(const std::string& filename) {
    fileStream_ = std::make_unique<std::ofstream>(filename);
    if (fileStream_->is_open()) {
        output_ = fileStream_.get();
    } else {
        output_ = &std::cout;
    }
}

std::string Logger::getColor(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            return "\033[32m";  // Green
        case LogLevel::ERROR:
            return "\033[31m";  // Red
        case LogLevel::DEBUG:
            return "\033[34m";  // Blue
        case LogLevel::WARN:
            return "\033[33m";  // Orange (Yellow)
        default:
            return "\033[0m";  // Reset
    }
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::WARN:
            return "WARN";
        default:
            return "UNKNOWN";
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << '.'
        << std::setfill('0') << std::setw(3) << now_ms.count();
    return oss.str();
}

void Logger::setLogLevel(LogLevel level) { currentLogLevel_ = level; }

}  // namespace utils
}  // namespace vrt