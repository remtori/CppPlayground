#include <ASL/Logger.h>

#include <iostream>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>

namespace ASL {

std::string Logger::s_process_name = "unknown";
std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::set_process_name(const char* arg0)
{
    // Get process base name from argv[0]
    {
        std::string s(arg0);
        auto i = s.find_last_of('/');
        s_process_name = s.substr(i + 1, s.find_last_of('.') - i - 1);
    }

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

} // namespace ASL