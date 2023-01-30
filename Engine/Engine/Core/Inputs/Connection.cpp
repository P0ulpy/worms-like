//
// Created by Admin on 30/01/2023.
//

#include "Connection.hpp"
#include "InputSignal.h"

Connection::Connection(Connection::Callback callback, Connection::StateType state, InputSignal* signal)
        : m_callback(callback), m_state(state), m_signal(signal)

{}

ScopedConnection::ScopedConnection() {

}

ScopedConnection::ScopedConnection(ScopedConnection&& other) {
    std::swap(m_connection, other.m_connection);
}

ScopedConnection& ScopedConnection::operator=(ScopedConnection&& other) {
    std::swap(m_connection, other.m_connection);
    return *this;
}

ScopedConnection::~ScopedConnection() {
    disconnect();
}

void ScopedConnection::disconnect() const {
    if (m_connection)
        m_connection->m_signal->Disconnect(m_eventType, m_connection);

}

SlotConnection::SlotConnection(InputSignal& signal, EventType eventType, const Connection::Callback& callback) :
        m_scopedConnection(signal.connectScoped(eventType, [=]() {
            if (!m_semaphore.isDisabled())
                return;

            callback();
        })),
        m_semaphore{}
{}

void SlotConnection::block() {
    m_semaphore.addBlocker();
}

void SlotConnection::unblock() {
    m_semaphore.removeBlocker();
}

auto SlotConnection::scopedBlock() {
    struct BlockScoped
    {
        explicit BlockScoped(SlotConnection& owner) : m_connection(owner)
        {
            m_connection.block();
        }

        ~BlockScoped()
        {
            m_connection.unblock();
        }

        SlotConnection& m_connection;
    };

    return BlockScoped{ *this };
}


