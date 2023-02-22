//
// Created by Admin on 22/02/2023.
//

#pragma once

#include "InputSignal.hpp"
#include "Connection.hpp"

template<typename Type>
class Observable {
public:
    Observable() = default;

    explicit Observable(const Type& value) : m_value(value) {}

    Observable(const Observable& other) : m_value(other.m_value) {}

    Observable& operator=(const Observable& other)
    {
        m_value = other.m_value;
        notify();
        return *this;
    }

    Observable& operator=(const Type& value)
    {
        m_value = value;
        notify();
        return *this;
    }

    void connect(std::function<void(const Type&)> callback)
    {
        m_scopedConnection = m_signal.connectScoped(callback);
    }

private:
    void notify()
    {
        m_signal(m_value);
    }

    Type m_value;
    InputSlot<Type> m_signal;
    ScopedConnection<Type> m_scopedConnection;
};
