//
// Created by Admin on 22/01/2023.
//

#ifndef POPOSIBENGINE_INPUTSIGNAL_H
#define POPOSIBENGINE_INPUTSIGNAL_H

#include <SFML/Window/Keyboard.hpp>

#include <functional>
#include <map>
#include <tuple>
#include <vector>
#include <variant>

#include "Connection.hpp"
#include "InputConfig.hpp"
#include <stdexcept>

class InputSignal {
    friend class InputConfig;

    using Callback = typename Connection::Callback;
    using Observers = std::vector<Connection*>;
    using ObserversMap = std::map<EventType, Observers>;

public:
    static InputSignal* GetInstance() {
        if (nullptr == m_inputSignal) {
            m_inputSignal = new InputSignal();
        }
        return m_inputSignal;
    }

    void connect(EventType eventType, const Callback& callback) const
    {
        auto* connection = new Connection(callback,
                                          typename Connection::StateType{ typename Connection::Managed {} },
                                          const_cast<InputSignal*>(this));
        m_observers[eventType].push_back(connection);
    }
    template<typename Type>
    void connect(EventType eventType, Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(eventType, callback);
    }
    template<typename Type>
    void connect(EventType eventType, Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(eventType, callback);
    }

    ScopedConnection connectScoped(EventType eventType, const Callback& callback) const
    {
        auto* connection = new Connection(callback,
                                          typename Connection::StateType{ typename Connection::Scoped {} },
                                          const_cast<InputSignal*>(this));
        m_observers[eventType].push_back(connection);
        return ScopedConnection{ eventType, connection };
    }
    template<typename Type>
    ScopedConnection connectScoped(EventType eventType, Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(eventType, callback);
    }
    template<typename Type>
    ScopedConnection connectScoped(EventType eventType, Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(eventType, callback);
    }

    SlotConnection connectSlot(EventType eventType, const Callback& callback) const
    {
        auto signal = const_cast<InputSignal*>(this);
        return SlotConnection{ *signal, eventType, callback };
    }
    template<typename Type>
    SlotConnection connectSlot(EventType eventType, Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(eventType, callback);
    }
    template<typename Type>
    SlotConnection connectSlot(EventType eventType, Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(eventType, callback);
    }

    void Disconnect(const EventType& eventType, Connection* connection) const {
        auto it = m_observers.find(eventType);
        if (it != m_observers.end())
        {
            auto connectionIt = std::find_if(it->second.begin(), it->second.end(),
                                             [&](Connection* c) { return c == connection; });

            if (connectionIt == it->second.end())
                throw std::runtime_error("Connection is not attached to this signal");

            match(connection->m_state,
                  [&](typename Connection::Managed) { throw std::runtime_error("Connection is managed typed"); },
                  [&](typename Connection::Scoped) { it->second.erase(connectionIt); },
                  [&](typename Connection::Zombified) {});

            delete connection;
            connection = nullptr;
        }
    }

    void Emit(const EventSignalType& eventSignalType) const {
        auto eventTypes = m_inputConfig->GetEventTypes(eventSignalType);

        if (eventTypes.empty())
            return;

        for (auto eventType : eventTypes)
        {
            auto it = m_observers.find(eventType);
            if (it != m_observers.end())
                for (auto observer : it->second)
                    observer->m_callback();
        }
    }

    void operator()(const EventSignalType& eventSignalType) const {
        Emit(eventSignalType);
    }

    void ChangeInputConfig(EventType eventType, EventSignalType eventSignalType)
    {
        auto PreviousEventSignalType = m_inputConfig->GetEventSignalType(eventType);

        if (PreviousEventSignalType == EventSignalType().Null())
            return;

        m_inputConfig->SetEventSignalType(eventType, eventSignalType);
    }

private:
    InputSignal()
    {
        m_inputConfig = std::make_unique<InputConfig>();
    }
    ~InputSignal()
    {
        for (auto& observer : m_observers) {
            for (auto& connection : observer.second) {
                match(connection->m_state,
                      [&](typename Connection::Managed) { delete connection; },
                      [&](typename Connection::Scoped) { connection->m_state = typename Connection::Zombified{}; },
                      [&](typename Connection::Zombified) { delete connection; });
            }
        }
    }


private:
    std::unique_ptr<InputConfig> m_inputConfig;
    static InputSignal* m_inputSignal;

    mutable ObserversMap m_observers;
};

#endif //POPOSIBENGINE_INPUTSIGNAL_H
