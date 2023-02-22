//
// Created by Admin on 30/01/2023.
//

#include "Connection.hpp"
#include "InputSignal.hpp"

namespace SignalSystem
{
    ConnectionSignal::ConnectionSignal(ConnectionSignal::Callback callback, ConnectionSignal::StateType state, InputSignal* signal)
            : m_callback(callback), m_state(state), m_signal(signal)

    {}

    ScopedConnectionSignal::ScopedConnectionSignal() {

    }

    ScopedConnectionSignal::ScopedConnectionSignal(ScopedConnectionSignal&& other) {
        std::swap(m_connection, other.m_connection);
    }

    ScopedConnectionSignal& ScopedConnectionSignal::operator=(ScopedConnectionSignal&& other) {
        std::swap(m_connection, other.m_connection);
        return *this;
    }

    ScopedConnectionSignal::~ScopedConnectionSignal() {
        disconnect();
    }

    void ScopedConnectionSignal::disconnect() const {
        if (nullptr != m_connection)
            m_connection->m_signal->Disconnect(m_eventType, m_connection);

    }

    SlotConnectionSignal::SlotConnectionSignal(InputSignal& signal, const EventType& eventType, const ConnectionSignal::Callback& callback) :
            m_scopedConnection(signal.connectScoped(eventType, [=]() {
                if (!m_semaphore.isDisabled())
                    return;

                callback();
            })),
            m_semaphore{}
    {}

    void SlotConnectionSignal::block() {
        m_semaphore.addBlocker();
    }

    void SlotConnectionSignal::unblock() {
        m_semaphore.removeBlocker();
    }

    auto SlotConnectionSignal::scopedBlock() {
        struct BlockScoped
        {
            explicit BlockScoped(SlotConnectionSignal& owner) : m_connection(owner)
            {
                m_connection.block();
            }

            ~BlockScoped()
            {
                m_connection.unblock();
            }

            SlotConnectionSignal& m_connection;
        };

        return BlockScoped{ *this };
    }


/*
 * REGION InputSlot
 */

    template<typename... Args>
    Connection<Args...>::Connection(Callback callback, StateType state, SignalPtr signal)
            : m_callback(callback), m_state(state), m_signal(signal)
    {}

    template<typename... Args>
    ScopedConnection<Args...>::ScopedConnection()
            : m_connection(nullptr)
    {}

    template<typename... Args>
    ScopedConnection<Args...>::ScopedConnection(ScopedConnection&& other) {
        std::swap(m_connection, other.m_connection);
    }

    template<typename... Args>
    ScopedConnection<Args...>& ScopedConnection<Args...>::operator=(ScopedConnection&& other) {
        std::swap(m_connection, other.m_connection);
        return *this;
    }

    template<typename... Args>
    ScopedConnection<Args...>::~ScopedConnection() {
        disconnect();
    }

    template<typename... Args>
    void ScopedConnection<Args...>::disconnect() const {
        if (nullptr != m_connection)
            m_connection->m_signal->Disconnect(m_connection);

    }

    template<typename... Args>
    SlotConnection<Args...>::SlotConnection(InputSignal &signal, const typename Connection<Args...>::Callback &callback)
            : m_scopedConnection(signal.connectScoped([=]() {
        if (!m_semaphore.isDisabled())
            return;

        callback();
    })),
              m_semaphore{}
    {}

    template<typename... Args>
    void SlotConnection<Args...>::block() {
        m_semaphore.addBlocker();
    }

    template<typename... Args>
    void SlotConnection<Args...>::unblock() {
        m_semaphore.removeBlocker();
    }

    template<typename... Args>
    auto SlotConnection<Args...>::scopedBlock() {
        struct BlockScoped
        {
            explicit BlockScoped(SlotConnection<Args...>& owner) : m_connection(owner)
            {
                m_connection.block();
            }

            ~BlockScoped()
            {
                m_connection.unblock();
            }

            SlotConnection<Args...>& m_connection;
        };

        return BlockScoped{ *this };
    }
}


