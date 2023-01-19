//
// Created by Flo on 17/01/2023.
//

#include "WindowEvents.hpp"

namespace Engine
{
    std::array<sf::Event, sf::Event::Count> WindowEvents::s_events;

    void WindowEvents::ProcessEvents(sf::RenderWindow &renderWindow)
    {
        clearEvents();

        sf::Event event {};
        while(renderWindow.pollEvent(event))
        {
#ifdef IMGUI_SUPPORT
            // ImGui Events Handling
            //ImGui::SFML::ProcessEvent(event);
#endif
            s_events[event.type] = event;
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
} // Engine