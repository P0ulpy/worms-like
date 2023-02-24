//
// Created by Flo on 28/11/2022.
//

#include "Logger.hpp"

#include <iostream>
#include <iomanip>

namespace Engine
{
    std::mutex Logger::_mutex {};
    std::unordered_map<std::thread::id, std::string> Logger::_threadsLabels {};

    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_INFO>()      { return { "INFO" }; }
    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_WARNING>()   { return { "WARN" }; }
    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_ERROR>()     { return { "ERR" }; }

    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_INFO>(std::ostream &s)      { return ConsoleColor::blue(s); }
    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_WARNING>(std::ostream &s)   { return ConsoleColor::yellow(s); }
    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_ERROR>(std::ostream &s)     { return ConsoleColor::red(s); }

    void Logger::SetThreadLabel(const std::string_view &label)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Logger::_threadsLabels[std::this_thread::get_id()] = label;
    }

    void Logger::SetThreadLabel(const std::string_view &label, std::thread::id threadID)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Logger::_threadsLabels[threadID] = label;
    }

    std::string Logger::CurrentDateTimeToString()
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::stringstream formatBuffer;
        formatBuffer << std::put_time(std::localtime(&now), "%H:%M:%S");

        return formatBuffer.str();
    }

} // Engine