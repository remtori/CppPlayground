#pragma once

#include <spdlog/spdlog.h>

#include <ASL/Forward.h>

#include <iostream>
#include <string>

#if defined(ASL_LOGGER_IMPL)
#    include <spdlog/fmt/ostr.h>
#    include <spdlog/sinks/basic_file_sink.h>
#    include <spdlog/sinks/stdout_color_sinks.h>
#endif

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

namespace ASL {

class Logger {
public:
    static void set_process_name(const char* arg0);
    inline static std::shared_ptr<spdlog::logger> logger() { return s_logger; }

private:
    static std::string s_process_name;
    static std::shared_ptr<spdlog::logger> s_logger;
};

#if defined(ASL_LOGGER_IMPL)

std::string Logger::s_process_name;
std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::set_process_name(const char* arg0)
{
    // Get process base name from argv[0]
    std::string s(arg0);
    auto i = s.find_last_of('/');
    s_process_name = s.substr(i + 1, s.find_last_of('.') - i - 1);

    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + s_process_name + ".log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%L] %n: %v");

    s_logger = std::make_shared<spdlog::logger>(s_process_name, logSinks.begin(), logSinks.end());
    spdlog::register_logger(s_logger);

    s_logger->set_level(spdlog::level::trace);
    s_logger->flush_on(spdlog::level::trace);
}

#endif

} // namespace ASL