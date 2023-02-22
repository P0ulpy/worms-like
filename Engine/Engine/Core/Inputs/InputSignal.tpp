#pragma once

namespace SignalSystem
{
    template<typename... Args>
    void InputSlot<Args...>::connect(const InputSlot::Callback &callback) const
    {
        auto* connection = new ConnectionType(callback,
                                              typename ConnectionType::StateType{ typename ConnectionType::Managed {} },
                                              const_cast<InputSlot<Args...>*>(this));
        m_observers.push_back(connection);
    }

    template<typename... Args>
    template<typename Type>
    void InputSlot<Args...>::connect(Type *instance, void (Type::*fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(callback);
    }

    template<typename... Args>
    template<typename Type>
    void InputSlot<Args...>::connect(Type *instance, void (Type::*fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        connect(callback);
    }

    template<typename... Args>
    typename InputSlot<Args...>::ScopedConnectionType InputSlot<Args...>::connectScoped(const Callback &callback) const
    {
        auto* connection = new ConnectionType (callback,
                                               typename ConnectionType::StateType{ typename ConnectionType::Scoped {} },
                                               const_cast<InputSlot<Args...>*>(this));
        m_observers.push_back(connection);
        return ScopedConnectionType{ connection };
    }

    template<typename... Args>
    template<typename Type>
    typename InputSlot<Args...>::ScopedConnectionType InputSlot<Args...>::connectScoped(Type *instance, void (Type::*fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(callback);
    }

    template<typename... Args>
    template<typename Type>
    typename InputSlot<Args...>::ScopedConnectionType InputSlot<Args...>::connectScoped(Type *instance, void (Type::*fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectScoped(callback);
    }

    template<typename... Args>
    typename InputSlot<Args...>::SlotConnectionType InputSlot<Args...>::connectSlot(const InputSlot::Callback &callback) const
    {
        auto signal = const_cast<InputSlot<Args...>*>(this);
        return SlotConnectionType{ *signal, callback };
    }

    template<typename... Args>
    template<typename Type>
    typename InputSlot<Args...>::SlotConnectionType InputSlot<Args...>::connectSlot(Type *instance, void (Type::*fn)()) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(callback);
    }

    template<typename... Args>
    template<typename Type>
    typename InputSlot<Args...>::SlotConnectionType InputSlot<Args...>::connectSlot(Type *instance, void (Type::*fn)() const) const
    {
        auto callback = [instance, fn]() { (instance->*fn)(); };
        return connectSlot(callback);
    }

    template<typename... Args>
    void InputSlot<Args...>::Disconnect(InputSlot::ConnectionType *connection) const
    {
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

    template<typename... Args>
    void InputSlot<Args...>::Emit(const Args &... args) const
    {
        auto connections = m_observers;
        for (auto& connection : connections)
            connection->m_callback(args...);
    }

    template<typename... Args>
    void InputSlot<Args...>::operator()(const Args &... args) const
    {
        Emit(args...);
    }
}