//
// Created by Flo on 28/11/2022.
//

#include "Logger.hpp"

#include <iostream>

namespace Engine
{
    std::mutex Logger::_mutex {};
    std::unordered_map<std::thread::id, std::string> Logger::_threadsLabels {};

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

} // Engine