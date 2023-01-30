//
// Created by Flo on 17/01/2023.
//

#ifndef POPOSIBENGINE_WINDOWEVENTS_HPP
#define POPOSIBENGINE_WINDOWEVENTS_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <functional>

namespace Engine
{
    enum class EventType
    {
        PlayerUp = 0,
        PlayerDown,
        PlayerLeft,
        PlayerRight,
        PlayerShoot,
        OpenInventory
    };

    struct EventSignalType
    {
        sf::Event::EventType eventType = sf::Event::EventType::Count;
        sf::Event::KeyEvent keyEvent = sf::Event::KeyEvent { sf::Keyboard::Key::KeyCount, true, true, true, true };

        bool operator==(const EventSignalType& other) const
        {
            return eventType == other.eventType && keyEvent.code == other.keyEvent.code;
        }

        bool operator<(const EventSignalType& other) const
        {
            return eventType < other.eventType && keyEvent.code < other.keyEvent.code;
        }
    };

    class InputConfig
    {
    public:
        void Init()
        {
            s_eventMap[EventType::PlayerUp] = EventSignalType { sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Z, false, false, false, false } };
            s_eventMap[EventType::PlayerDown] = EventSignalType { sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::S, false, false, false, false } };
            s_eventMap[EventType::PlayerLeft] = EventSignalType { sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::Q, false, false, false, false } };
            s_eventMap[EventType::PlayerRight] = EventSignalType { sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::D, false, false, false, false } };
            s_eventMap[EventType::PlayerShoot] = EventSignalType { sf::Event::EventType::MouseLeft, sf::Event::KeyEvent { sf::Keyboard::Key::KeyCount, true, true, true, true } };
            s_eventMap[EventType::OpenInventory] = EventSignalType { sf::Event::EventType::KeyPressed, sf::Event::KeyEvent { sf::Keyboard::Key::I, false, false, false, false } };
        }

        EventSignalType GetEventSignalType(EventType eventType)
        {
            return s_eventMap[eventType];
        }

        void SetEventSignalType(EventType eventType, EventSignalType eventSignalType)
        {
            s_eventMap[eventType] = eventSignalType;
        }

    public:
        InputConfig()
        {
            Init();
        }

        ~InputConfig()
        {
            s_eventMap.clear();
        }

    private:
        std::map<EventType, EventSignalType> s_eventMap;
    };

    class InputSignal {
        friend class InputConfig;

        using Observer = std::function<void()>;
        using Observers = std::vector<std::pair<size_t, Observer>>;
        using ObserversMap = std::map<EventSignalType, Observers>;

    public:
        static InputSignal* GetInstance()
        {
            if (nullptr == m_inputSignal)
            {
                m_inputSignal = new InputSignal();
            }

            return m_inputSignal;
        }

        size_t Connect(const EventSignalType& eventSignalType, const Observer& observer) const
        {
            m_observers[eventSignalType].emplace_back(++m_currentId, observer);
            return m_currentId;
        }

        void Disconnect(const EventSignalType& eventSignalType, size_t id) const
        {
            auto it = m_observers.find(eventSignalType);

            if(it != m_observers.end())
            {
                for(size_t i = it->second.size() - 1; i > 0; --i)
                {
                    if(it->second[i].first == id)
                    {
                        it->second.erase(it->second.end() - i);
                        break;
                    }
                }
            }
        }

        void Emit(const EventSignalType& eventSignalType) const
        {
            auto it = m_observers.find(eventSignalType);

            if(it != m_observers.end())
            {
                for(auto& observer : it->second)
                {
                    observer.second();
                }
            }
        }

        void ChangeInputConfig(EventType eventType, EventSignalType eventSignalType)
        {
            auto PreviousEventSignalType = m_inputConfig->GetEventSignalType(eventType);

            auto it = m_observers.find(PreviousEventSignalType);
            auto vector = it->second;

            m_observers.erase(it);
            m_observers[eventSignalType] = vector;

            m_inputConfig->SetEventSignalType(eventType, eventSignalType);
        }

    private:
        InputSignal()
        {
            m_inputConfig = std::make_unique<InputConfig>();
        }
        ~InputSignal()
        {
            m_observers.clear();
        }


    private:
        std::unique_ptr<InputConfig> m_inputConfig;
        static InputSignal* m_inputSignal;

        mutable ObserversMap m_observers;
        mutable size_t m_currentId = 48;
    };


    class WindowEvents
    {
    public:
        static void ProcessEvents(sf::RenderWindow& renderWindow);

        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType);
        [[nodiscard]] static bool GetEvent(sf::Event::EventType eventType, sf::Event& outEvent);

    private:
        static void clearEvents();

    private:
        static std::array<sf::Event, sf::Event::Count> s_events;
    };

} // Engine

#endif //POPOSIBENGINE_WINDOWEVENTS_HPP
