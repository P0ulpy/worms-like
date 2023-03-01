//
// Created by Flo on 17/01/2023.
//

#include "WindowEvents.hpp"
#include "InputSignal.hpp"

#include <iostream>

namespace Engine
{
    std::array<sf::Event, sf::Event::Count> WindowEvents::s_events;
    sf::Vector2i WindowEvents::s_mousePixelPosition;
    sf::Vector2f WindowEvents::s_mouseWorldPosition;

    void WindowEvents::ProcessEvents(sf::RenderWindow &renderWindow)
    {
        clearEvents();

        sf::Event event {};
        s_mousePixelPosition = sf::Mouse::getPosition(renderWindow);
        s_mouseWorldPosition = renderWindow.mapPixelToCoords(s_mousePixelPosition);

        while(renderWindow.pollEvent(event))
        {
#ifdef IMGUI_SUPPORT
            // ImGui Events Handling
            //ImGui::SFML::ProcessEvent(event);
#endif
            s_events[event.type] = event;

            auto mouse = SignalSystem::InputConfig::Get()->GetMouseBindingName(event.type, event.mouseButton.button);
            if (!mouse.empty())
                for(const auto& name : mouse)
                    SignalSystem::InputSignal::Get()->Emit(name);

            auto key = SignalSystem::InputConfig::Get()->GetKeyBindingName(event.type, event.key.code);
            if (!key.empty())
                for(const auto& name : key)
                    SignalSystem::InputSignal::Get()->Emit(name);

            auto eventName = SignalSystem::InputConfig::Get()->GetEventBindingName(event.type);
            if (!eventName.empty())
                SignalSystem::InputSignal::Get()->Emit(eventName);
        }
    }

    bool WindowEvents::GetEvent(sf::Event::EventType eventType)
    {
        return (s_events[eventType].type != sf::Event::Count);
    }

    bool WindowEvents::GetEvent(sf::Event::EventType eventType, sf::Event &outEvent)
    {
        if (s_events[eventType].type != sf::Event::Count)
        {
            outEvent = s_events[eventType];
            return true;
        }

        return false;
    }

    void WindowEvents::clearEvents()
    {
        for (auto& event : s_events)
        {
            event.type = sf::Event::Count;
        }
    }

    sf::Vector2i WindowEvents::GetMousePixelPosition() {
        return s_mousePixelPosition;
    }

    sf::Vector2f WindowEvents::GetMouseWorldPosition() {
        return s_mouseWorldPosition;
    }
} // Engine