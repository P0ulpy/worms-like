//
// Created by Admin on 22/01/2023.
//

#pragma once

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
    using ConnectionType = ConnectionSignal;
    using ScopedConnectionType = ScopedConnectionSignal;
    using SlotConnectionType = SlotConnectionSignal;

    using EventType = std::string_view;
    using Callback = typename ConnectionType::Callback;
    using Observers = std::vector<ConnectionType*>;
    using ObserversMap = std::map<EventType, Observers>;

public:
    static InputSignal* Get() {
        if (nullptr == m_inputSignal) {
            m_inputSignal = new InputSignal();
        }
        return m_inputSignal;
    }

    void connect(EventType eventType, const Callback& callback) const
    {
        auto* connection = new ConnectionType(callback,
                                          typename ConnectionType::StateType{ typename ConnectionType::Managed {} },
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

    ScopedConnectionType connectScoped(EventType eventType, const Callback& callback) const
    {
        auto* connection = new ConnectionType (callback,
                                          typename ConnectionType::StateType{ typename ConnectionType::Scoped {} },
                                          const_cast<InputSignal*>(this));
        m_observers[eventType].push_back(connection);
        return ScopedConnectionType{ eventType, connection };
    }
    template<typename Type>
    ScopedConnectionType connectScoped(EventType eventType, Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(eventType, callback);
    }
    template<typename Type>
    ScopedConnectionType connectScoped(EventType eventType, Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(eventType, callback);
    }

    SlotConnectionType connectSlot(EventType eventType, const Callback& callback) const
    {
        auto signal = const_cast<InputSignal*>(this);
        return SlotConnectionType{ *signal, eventType, callback };
    }
    template<typename Type>
    SlotConnectionType connectSlot(EventType eventType, Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(eventType, callback);
    }
    template<typename Type>
    SlotConnectionType connectSlot(EventType eventType, Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(eventType, callback);
    }

    void Disconnect(const EventType& eventType, ConnectionType* connection) const {
        auto it = m_observers.find(eventType);
        if (it != m_observers.end())
        {
            auto connectionIt = std::find_if(it->second.begin(), it->second.end(),
                                             [&](ConnectionType* c) { return c == connection; });

            if (connectionIt == it->second.end())
                throw std::runtime_error("Connection is not attached to this signal");

            match(connection->m_state,
                  [&](typename ConnectionType::Managed) { throw std::runtime_error("Connection is managed typed"); },
                  [&](typename ConnectionType::Scoped) { it->second.erase(connectionIt); },
                  [&](typename ConnectionType::Zombified) {});

            delete connection;
            connection = nullptr;
        }
    }

    void Emit(const EventType& eventType) const {
        if(eventType.empty())
            return;

        auto it = m_observers.find(eventType);
        if (it != m_observers.end())
        {
            auto observers = it->second;
            for (auto observer : observers)
                observer->m_callback();
        }
    }

    void operator()(const EventType& eventType) const {
        Emit(eventType);
    }

private:
    InputSignal() = default;
    ~InputSignal()
    {
        for (auto& observer : m_observers) {
            for (auto& connection : observer.second) {
                match(connection->m_state,
                      [&](typename ConnectionType::Managed) { delete connection; },
                      [&](typename ConnectionType::Scoped) { connection->m_state = typename ConnectionType::Zombified{}; },
                      [&](typename ConnectionType::Zombified) { delete connection; });
            }
        }
    }


private:
    static InputSignal* m_inputSignal;

    mutable ObserversMap m_observers;

};

template<typename... Args>
class InputSlot {
    using ConnectionType = Connection<Args...>;
    using ScopedConnectionType = ScopedConnection<Args...>;
    using SlotConnectionType = SlotConnection<Args...>;

    using Callback = typename ConnectionType::Callback;
    using Observers = std::vector<ConnectionType*>;

public:
    InputSlot() = default;
    ~InputSlot()
    {
        for (auto& connection : m_observers) {
            match(connection->m_state,
                  [&](typename ConnectionType::Managed) { delete connection; },
                  [&](typename ConnectionType::Scoped) { connection->m_state = typename ConnectionType::Zombified{}; },
                  [&](typename ConnectionType::Zombified) { delete connection; });
        }
    }

    void connect(const Callback& callback) const
    {
        auto* connection = new ConnectionType(callback,
                                              typename ConnectionType::StateType{ typename ConnectionType::Managed {} },
                                              const_cast<InputSignal*>(this));
        m_observers.push_back(connection);
    }
    template<typename Type>
    void connect(Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(callback);
    }
    template<typename Type>
    void connect(Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(callback);
    }

    ScopedConnectionType connectScoped(const Callback& callback) const
    {
        auto* connection = new ConnectionType (callback,
                                               typename ConnectionType::StateType{ typename ConnectionType::Scoped {} },
                                               const_cast<InputSignal*>(this));
        m_observers.push_back(connection);
        return ScopedConnectionType{ connection };
    }
    template<typename Type>
    ScopedConnectionType connectScoped(Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(callback);
    }
    template<typename Type>
    ScopedConnectionType connectScoped(Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(callback);
    }

    SlotConnectionType connectSlot(const Callback& callback) const
    {
        auto signal = const_cast<InputSignal*>(this);
        return SlotConnectionType{ *signal, callback };
    }
    template<typename Type>
    SlotConnectionType connectSlot(Type* instance, void (Type::* fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(callback);
    }
    template<typename Type>
    SlotConnectionType connectSlot(Type* instance, void (Type::* fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(callback);
    }

    void Disconnect(ConnectionType* connection) const {
        auto connectionIt = std::find_if(m_observers.begin(), m_observers.end(),
                                         [&](ConnectionType* c) { return c == connection; });

        if (connectionIt == m_observers.end())
            throw std::runtime_error("Connection is not attached to this signal");

        match(connection->m_state,
              [&](typename ConnectionType::Managed) { throw std::runtime_error("Connection is managed typed"); },
              [&](typename ConnectionType::Scoped) { m_observers.erase(connectionIt); },
              [&](typename ConnectionType::Zombified) {});

        delete connection;
        connection = nullptr;
    }

    void Emit(const Args&... args) const {
        auto connections = m_observers;
        for (auto& connection : connections)
            connection->fn(args...);
    }

    void operator()(const Args&... args) const {
        Emit(args...);
    }

private:

private:
    static InputSignal* m_inputSignal;

    mutable Observers m_observers;

};
