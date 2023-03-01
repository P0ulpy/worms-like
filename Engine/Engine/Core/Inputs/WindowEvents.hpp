//
// Created by Flo on 17/01/2023.
//

#ifndef POPOSIBENGINE_WINDOWEVENTS_HPP
#define POPOSIBENGINE_WINDOWEVENTS_HPP

#include "SFML/Graphics.hpp"
#include <array>
#include <functional>

namespace Engine
{
    class WindowEvents
    {
    public:
        static void ProcessEvents(sf::RenderWindow& renderWindow);

        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType);
        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType, sf::Event& outEvent);
        [[nodiscard]] static sf::Vector2i GetMousePixelPosition();
        [[nodiscard]] static sf::Vector2f GetMouseWorldPosition();

    private:
        static void clearEvents();

    private:
        static std::array<sf::Event, sf::Event::Count> s_events;
        static sf::Vector2i s_mousePixelPosition;
        static sf::Vector2f s_mouseWorldPosition;
    };

} // Engine

#endif //POPOSIBENGINE_WINDOWEVENTS_HPP
