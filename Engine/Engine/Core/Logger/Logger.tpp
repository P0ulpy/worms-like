//
// Created by Flo on 28/11/2022.
//

#ifndef PATHFINDER_LOGGER_TPP
#define PATHFINDER_LOGGER_TPP
/*
#include <iomanip>
#include <sstream>
#include "ConsoleColor.hpp"

namespace Engine
{
    template <LogType type> std::string_view LogTypeLabel();
    template <> std::string_view LogTypeLabel<LogType::LOG_INFO>() { return "INFO"; }
    template <> std::string_view LogTypeLabel<LogType::LOG_WARNING>() { return "WARN"; }
    template <> std::string_view LogTypeLabel<LogType::LOG_ERROR>() { return "ERR"; }

    template <LogType type> std::ostream& LogTypeColor(std::ostream &s);
    template <> std::ostream& LogTypeColor<LogType::LOG_INFO>(std::ostream &s) { return ConsoleColor::blue(s); }
    template <> std::ostream& LogTypeColor<LogType::LOG_WARNING>(std::ostream &s) { return ConsoleColor::yellow(s); }
    template <> std::ostream& LogTypeColor<LogType::LOG_ERROR>(std::ostream &s) { return ConsoleColor::red(s); }

    namespace
    {
        std::string CurrentDateTimeToString()
        {
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            //std::string output(30, '\0');
            //std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));

            std::stringstream formatBuffer;
            formatBuffer << std::put_time(std::localtime(&now), "%H:%M:%S");

            return formatBuffer.str();
        }
    }

    template<LogType logType, typename... Args>
    LogEntry Logger::PrintMessage(std::ostream& ostream, Args... args)
    {
        LogEntry logEntry = ConstructEntry<logType>();

        std::stringstream messageBuffer;
        BuildMessage<Args...>(messageBuffer, args...);
        logEntry.message += messageBuffer.str();

        std::string_view threadLabel = (_threadsLabels.contains(logEntry.threadId))
                                       ? _threadsLabels.at(logEntry.threadId)
                                       : std::string_view("");

        ostream << ConsoleColor::blue
                << '[';

        if(!threadLabel.empty())
            ostream << threadLabel;
        else
            ostream << logEntry.threadId;

        ostream << ']'
                << logEntry.message
                << ConsoleColor::reset
                << std::endl;

        return logEntry;
    }

    template<typename ...Args>
    void Logger::Log(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_INFO, Args...>(std::cout, args...);

        //messages.push_back(logEntry);
    }

    template<typename ...Args>
    void Logger::Warn(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_WARNING, Args...>(std::cout, args...);

        //messages.push_back(logEntry);
    }

    template<typename ...Args>
    void Logger::Err(Args... args)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto logEntry = PrintMessage<LOG_ERROR, Args...>(std::cerr, args...);

        //messages.push_back(logEntry);
    }

    template <LogType logType>
    LogEntry Logger::ConstructEntry()
    {
        LogEntry logEntry;
        logEntry.type = logType;
        logEntry.threadId = std::this_thread::get_id();

        std::stringstream headerBuffer;
        headerBuffer << "["
                     << CurrentDateTimeToString()
                     << "] "
                     << LogTypeLabel<logType>()
                     << ": ";

        return logEntry;
    }

    template<typename FirstMessage, typename ...Args>
    void Logger::BuildMessage(std::stringstream& out, const FirstMessage& first, Args... args)
    {
        out << ' ' << first;

        if constexpr (sizeof...(args) > 0)
            BuildMessage(out, args...);
    }

} // Engine
*/
#endif //PATHFINDER_LOGGER_TPP