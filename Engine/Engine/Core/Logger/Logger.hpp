//
// Created by Flo on 28/11/2022.
//

#pragma once

#include "ConsoleColor.hpp"

#include <thread>
#include <string>
#include <mutex>
#include <unordered_map>
#include <sstream>

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
        static std::string CurrentDateTimeToString();

        template <LogType type> static std::string_view LogTypeLabel();
        template <LogType type> static std::ostream& LogTypeColor(std::ostream &s);

        template<LogType logType, typename ...Args>
        static LogEntry PrintMessage(std::ostream& ostream, Args... args);

        template <LogType logType>
        static LogEntry ConstructEntry();

        template<typename FirstMessage, typename ...Args>
        static void BuildMessage(std::stringstream& out, const FirstMessage& first, Args... args);

        static std::mutex _mutex;
        static std::unordered_map<std::thread::id, std::string> _threadsLabels;
    };

    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_INFO>();
    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_WARNING>();
    template <> std::string_view Logger::LogTypeLabel<LogType::LOG_ERROR>();

    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_INFO>(std::ostream &s);
    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_WARNING>(std::ostream &s);
    template <> std::ostream& Logger::LogTypeColor<LogType::LOG_ERROR>(std::ostream &s);

} // Engine

#include "Logger.tpp"