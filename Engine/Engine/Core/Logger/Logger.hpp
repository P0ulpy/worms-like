//
// Created by Flo on 28/11/2022.
//

#ifndef PATHFINDER_LOGGER_HPP
#define PATHFINDER_LOGGER_HPP

/*#include <unordered_map>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>

#include "ConsoleColor.hpp"*/

#include <thread>
#include <string>
#include <mutex>
#include <unordered_map>

namespace Engine
{
    enum LogType {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    struct LogEntry
    {
        LogType type;
        std::thread::id threadId;
        std::string message;
    };

    class Logger
    {
    public:
        template<typename ...Args>
        static void Log(Args... args);

        template<typename ...Args>
        static void Warn(Args... args);

        template<typename ...Args>
        static void Err(Args... args);

        static void SetThreadLabel(const std::string_view& label);
        static void SetThreadLabel(const std::string_view& label, std::thread::id threadID);

    private:
        template<LogType logType, typename ...Args>
        static LogEntry PrintMessage(std::ostream& ostream, Args... args);

        template <LogType logType>
        static LogEntry ConstructEntry();

        template<typename FirstMessage, typename ...Args>
        static void BuildMessage(std::stringstream& out, const FirstMessage& first, Args... args);

        static std::mutex _mutex;
        static std::unordered_map<std::thread::id, std::string> _threadsLabels;
    };
} // Engine

#include "Logger.tpp"

#endif //PATHFINDER_LOGGER_HPP
