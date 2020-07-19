#pragma once

#include <spdlog/spdlog.h>

#include <ASL/Forward.h>
#include <spdlog/fmt/ostr.h>

namespace ASL {

class LIB_API Logger {
public:
    static void set_process_name(const char* arg0);
    inline static std::shared_ptr<spdlog::logger> logger() { return s_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
    static std::string s_process_name;
};

} // namespace ASL

#ifndef DISABLE_LOGGING

#    define LOG_TRACE(...) ::ASL::Logger::logger()->trace(__VA_ARGS__)
#    define LOG_INFO(...) ::ASL::Logger::logger()->info(__VA_ARGS__)
#    define LOG_WARN(...) ::ASL::Logger::logger()->warn(__VA_ARGS__)
#    define LOG_ERROR(...) ::ASL::Logger::logger()->error(__VA_ARGS__)
#    define LOG_CRITICAL(...) ::ASL::Logger::logger()->critical(__VA_ARGS__)

#else

#    define LOG_TRACE(...)
#    define LOG_INFO(...)
#    define LOG_WARN(...)
#    define LOG_ERROR(...)
#    define LOG_CRITICAL(...)

#endif