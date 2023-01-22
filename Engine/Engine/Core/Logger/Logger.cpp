//
// Created by Flo on 28/11/2022.
//

#include "Logger.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "ConsoleColor.hpp"

std::string CurrentDateTimeToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    //std::string output(30, '\0');
    //std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));

    std::stringstream formatBuffer;
    formatBuffer << std::put_time(std::localtime(&now), "%H:%M:%S");

    return formatBuffer.str();
}

namespace Engine
{
    std::mutex Logger::_mutex;
    std::list<LogEntry> Logger::messages;
    std::unordered_map<std::thread::id, std::string> Logger::_threadsLabels;

    template<typename ...Types>
    void Logger::Log(const Types&... message)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        LogEntry logEntry = ConstructEntry(LOG_INFO, "");
        BuildMessage(message..., logEntry.message);

        std::string threadLabel = (_threadsLabels.contains(logEntry.threadId)) ? _threadsLabels.at(logEntry.threadId) : "";

        std::cout << ConsoleColor::blue
                  << '[';

        if(!threadLabel.empty()) std::cout << threadLabel;
        else std::cout << logEntry.threadId;

        std::cout << ']'
                  << logEntry.message
                  << ConsoleColor::white
                  << std::endl;

        messages.push_back(logEntry);
    }

    template<typename ...Types>
    void Logger::Warn(const Types&... message)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        LogEntry logEntry = ConstructEntry(LOG_WARNING, "WARN");
        BuildMessage(message..., logEntry.message);

        std::string threadLabel = (Logger::_threadsLabels.contains(logEntry.threadId)) ? Logger::_threadsLabels.at(logEntry.threadId) : "";

        std::cout << ConsoleColor::yellow
                  << '[';

        if(!threadLabel.empty()) std::cout << threadLabel;
        else std::cout << logEntry.threadId;

        std::cout << ']'
                  << logEntry.message
                  << ConsoleColor::white
                  << std::endl;

        messages.push_back(logEntry);
    }

    template<typename ...Types>
    void Logger::Err(const Types&... message)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        LogEntry logEntry = ConstructEntry(LOG_ERROR, "ERR");
        BuildMessage(message..., logEntry.message);

        std::string threadLabel = (_threadsLabels.contains(logEntry.threadId)) ? Logger::_threadsLabels[logEntry.threadId] : "";

        std::cerr << ConsoleColor::red
                  << '[';

        if(!threadLabel.empty()) std::cerr << threadLabel;
        else std::cerr << logEntry.threadId;

        std::cerr << ']'
                  << logEntry.message
                  << ConsoleColor::white
                  << std::endl;

        messages.push_back(logEntry);
    }

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

    LogEntry Logger::ConstructEntry(LogType type, const std::string& header) {
        LogEntry logEntry;
        logEntry.type = type;
        logEntry.threadId = std::this_thread::get_id();
        logEntry.message = "[" + CurrentDateTimeToString() + "] " + header +": ";

        return logEntry;
    }

    template<typename FirstMessage, typename ...Messages>
    void Logger::BuildMessage(const FirstMessage& first, const Messages&... messages, std::string& base) {

        static_assert(std::is_same<FirstMessage, std::string_view>::value);

        base = base + first;
        if constexpr (sizeof...(messages) > 0)
        BuildMessage(messages..., base);
    }
} // Engine