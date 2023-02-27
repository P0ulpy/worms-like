//
// Created by Admin on 30/01/2023.
//

#pragma once

#include <vector>
#include <utility>

#include "Binding.hpp"
#include "../../Vendor/mINI/ini.hpp"

namespace SignalSystem
{
    class InputConfig {
        using ConfigKey = std::string;
        using ConfigType = uint64_t;
    public:
        static InputConfig* Get()
        {
            static InputConfig m_inputConfig;

            return &m_inputConfig;
        }

        void LoadConfig(const std::string& path)
        {
            mINI::INIFile file(path);

            mINI::INIStructure ini;
            file.read(ini);
            auto keys = ini["keys"];
            for (auto& key : keys)
            {
                auto binding = BindingsMap.at(key.second);
                m_bindingskeys.emplace(key.first, binding);
            }

            auto mouse = ini["mouse"];
            for (auto& key : mouse)
            {
                auto binding = BindingsMap.at(key.second);
                m_bindingsmouse.emplace(key.first, binding);
            }

            auto events = ini["events"];
            for (auto& key : events)
            {
                auto binding = BindingsMap.at(key.second);
                m_bindingsevents.emplace(key.first, binding);
            }

        }

        [[nodiscard]] ConfigType GetKeyBinding(const ConfigKey& eventType) const { return m_bindingskeys.at(eventType); }
        [[nodiscard]] ConfigType GetMouseBinding(const ConfigKey& eventType) const { return m_bindingsmouse.at(eventType); }
        [[nodiscard]] std::vector<ConfigKey> GetKeyBindingName(const ConfigType& event, const ConfigType& binding) const
        {
            std::vector<ConfigKey> names;

            if(m_bindingskeys.empty())
                return names;

            if(event != sf::Event::EventType::KeyPressed && event != sf::Event::EventType::KeyReleased)
                return names;

            for (auto& key : m_bindingskeys)
            {
                if (key.second == binding)
                    if(m_bindingsevents.at(key.first) == event)
                        names.push_back(key.first);
            }

            return names;
        }
        [[nodiscard]] std::vector<ConfigKey> GetMouseBindingName(const ConfigType& event, const ConfigType& binding) const
        {
            std::vector<ConfigKey> names;

            if(m_bindingsmouse.empty())
                return names;

            if(event != sf::Event::EventType::MouseWheelMoved && event != sf::Event::EventType::MouseWheelScrolled
               && event != sf::Event::EventType::MouseButtonPressed && event != sf::Event::EventType::MouseButtonReleased
               && event != sf::Event::EventType::MouseMoved && event != sf::Event::EventType::MouseEntered
               && event != sf::Event::EventType::MouseLeft)
                return names;

            for (auto& key : m_bindingsmouse)
            {
                if (key.second == binding)
                    if(m_bindingsevents.at(key.first) == event)
                        names.push_back(key.first);
            }
            return names;
        }
        [[nodiscard]] ConfigKey GetEventBindingName(const ConfigType& event) const
        {
            if(m_bindingsevents.empty())
                return "";

            if(event == sf::Event::EventType::KeyPressed || event == sf::Event::EventType::KeyReleased
               || event == sf::Event::EventType::MouseWheelMoved || event == sf::Event::EventType::MouseWheelScrolled
               || event == sf::Event::EventType::MouseButtonPressed || event == sf::Event::EventType::MouseButtonReleased
               || event == sf::Event::EventType::MouseMoved || event == sf::Event::EventType::MouseEntered
               || event == sf::Event::EventType::MouseLeft)
                return "";

            for (auto& key : m_bindingsevents)
            {
                if (key.second == event)
                    return key.first;
            }
            return "";
        }

    private:
        explicit InputConfig() = default;

        std::unordered_map<ConfigKey, ConfigType> m_bindingskeys;
        std::unordered_map<ConfigKey, ConfigType> m_bindingsmouse;
        std::unordered_map<ConfigKey, ConfigType> m_bindingsevents;
    };
}
