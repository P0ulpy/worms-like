//
// Created by Admin on 30/01/2023.
//

#pragma once

#include <vector>
#include <utility>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>


enum class EventType
{
    PlayerUp = 0,
    PlayerDown,
    PlayerLeft,
    PlayerRight,
    PlayerShoot,
    OpenInventory,
    CloseWindow,
    FullScreen,
    Pause,
    Click,
    Count
};

struct EventSignalType
{
    sf::Event::EventType eventType = sf::Event::EventType::Count;
    sf::Event::KeyEvent keyEvent = sf::Event::KeyEvent{ sf::Keyboard::Key::KeyCount, true, true, true, true };

    bool operator==(const EventSignalType& other) const
    {
        return eventType == other.eventType && keyEvent.code == other.keyEvent.code && keyEvent.alt == other.keyEvent.alt && keyEvent.control == other.keyEvent.control && keyEvent.shift == other.keyEvent.shift && keyEvent.system == other.keyEvent.system;
    }

    EventSignalType Null()
    {
        return EventSignalType{ sf::Event::EventType::Count, sf::Event::KeyEvent { sf::Keyboard::Key::KeyCount, true, true, true, true } };
    }
};

class InputConfig {
public:
    void Init()
    {
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Z, false, false, false, false } }, EventType::PlayerUp );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::S, false, false, false, false } }, EventType::PlayerDown );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Q, false, false, false, false } }, EventType::PlayerLeft );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::D, false, false, false, false } }, EventType::PlayerRight );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::MouseButtonPressed, sf::Event::KeyEvent { sf::Keyboard::Key::KeyCount, true, true, true, true } }, EventType::PlayerShoot );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::E, false, false, false, false } }, EventType::OpenInventory );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Enter, false, false, false, false } }, EventType::CloseWindow );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::F11, false, false, false, false } }, EventType::FullScreen );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Escape, false, false, false, false } }, EventType::Pause );
        s_events.emplace_back( EventSignalType{ sf::Event::EventType::MouseButtonPressed, sf::Event::KeyEvent { sf::Keyboard::Key::KeyCount, true, true, true, true } }, EventType::Click );
    }
    EventSignalType GetEventSignalType(EventType eventType)
    {
        for (auto pairEvent : s_events)
            if (pairEvent.second == eventType)
                return pairEvent.first;

        return EventSignalType().Null();
    }

    std::vector<EventType> GetEventTypes(EventSignalType eventSignalType)
    {
        std::vector<EventType> v_eventType;

        for (auto pairEvent : s_events)
            if (pairEvent.first == eventSignalType)
                v_eventType.push_back(pairEvent.second);

        return v_eventType;
    }
    void SetEventSignalType(EventType eventType, EventSignalType eventSignalType)
    {
        for (auto it = s_events.begin(); it != s_events.end(); )
        {
            if (it->second == eventType)
                it = s_events.erase(it);
            else
                ++it;
        }

        s_events.emplace_back( eventSignalType, eventType );
    }
public:
    InputConfig()
    {
        Init();
    }

    ~InputConfig()
    {
        s_events.clear();
    }

private:
    std::vector<std::pair<EventSignalType, EventType>> s_events;
};
