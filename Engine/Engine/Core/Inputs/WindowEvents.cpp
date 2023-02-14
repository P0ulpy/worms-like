//
// Created by Flo on 17/01/2023.
//

#include "WindowEvents.hpp"
#include "InputSignal.h"

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

            if((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseLeft) && event.mouseButton.button == sf::Mouse::Button::Left)
                InputSignal::GetInstance()->Emit({ event.type, { sf::Keyboard::Key::KeyCount, true, true, true, true }});
            else
                InputSignal::GetInstance()->Emit({ event.type, event.key});
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