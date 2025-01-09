#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <bitset>

namespace vrt {
    namespace utils {
        enum class LogLevel {
            INFO,
            ERROR,
            DEBUG
        };

        class Logger {
        public:
            static void setLogLevel(LogLevel level);
            static void setOutput(const std::string& filename);
            template<typename... Args>
            static void log(LogLevel level, const char* function, const char* format, Args... args) {
                if (level > currentLogLevel_) {
                    return;
                }
                std::string color = getColor(level);
                std::string resetColor = "\033[0m";
                std::string levelStr = getLevelString(level);
                std::string currentTime = getCurrentTime();
                std::string message = formatString(format, std::forward<Args>(args)...);
                (*output_) << color << "[" << currentTime << "] [" << std::setw(5) << std::left << levelStr << "] "
                        << std::setw(80) << std::left << function << resetColor << ": " << message << std::endl;
            }

        private:
            static std::unique_ptr<std::ofstream> fileStream_;
            static std::ostream* output_;
            static LogLevel currentLogLevel_;
            static std::string getColor(LogLevel level);
            static std::string getLevelString(LogLevel level);
            static std::string getCurrentTime();

            static inline void formatStringHelper(std::ostringstream& oss, const char* format) {
            while (*format) {
                if (*format == '{' && *(format + 1) == '}') {
                    throw std::runtime_error("Too few arguments provided for format string");
                }
                oss << *format++;
            }
        }

            template<typename T, typename... Args>
            static void formatStringHelper(std::ostringstream& oss, const char* format, T value, Args&&... args) {
                while (*format) {
                    if (*format == '{' && *(format + 1) == '}') {
                        oss << value;
                        format += 2;
                        formatStringHelper(oss, format, std::forward<Args>(args)...);
                        return;
                    } else if (*format == '{' && *(format + 1) == 'x' && *(format + 2) == '}') {
                        oss << std::hex << std::showbase << value;
                        format += 3;
                        formatStringHelper(oss, format, std::forward<Args>(args)...);
                        return;
                    } else if (*format == '{' && *(format + 1) == 'b' && *(format + 2) == '}') {
                        oss << "0b" << std::bitset<sizeof(T) * 8>(value);
                        format += 3;
                        formatStringHelper(oss, format, std::forward<Args>(args)...);
                        return;
                    } else if (*format == '{' && *(format + 1) == 'o' && *(format + 2) == '}') {
                        oss << "0o" << std::oct << std::showbase << value;
                        format += 3;
                        formatStringHelper(oss, format, std::forward<Args>(args)...);
                        return;
                    }
                    oss << *format++;
                }
                throw std::runtime_error("Too many arguments provided for format string");

            
            }

            template<typename... Args>
            static std::string formatString(const char* format, Args&&... args) {
                std::ostringstream oss;
                formatStringHelper(oss, format, std::forward<Args>(args)...);
                return oss.str();
        }
        };

    } // namespace utils
} // namespace vrt

#endif // LOGGER_HPP
