//
// Created by Admin on 30/01/2023.
//

#ifndef POPOSIBENGINE_CONNECTION_HPP
#define POPOSIBENGINE_CONNECTION_HPP


#include <functional>
#include <variant>

class InputSignal;
enum class EventType;

template<typename... Types>
struct Overload : Types... {
    using Types::operator()...;
};

template<typename... Types>
Overload(Types...)->Overload<Types...>;

template<typename Variant, typename... Matchers>
auto match(Variant&& variant, Matchers&&... matchers) {
    return std::visit(Overload{ std::forward<Matchers>(matchers)... }, variant);
}

struct Semaphore
{
    Semaphore() : m_counter(0) {}

    void addBlocker() { m_counter++; }
    void removeBlocker() { --m_counter; }
    bool isDisabled() const { return m_counter == 0; }

private:
    size_t m_counter;
};

struct Connection {
    friend class InputSignal;

    using Callback = std::function<void()>;
    struct Managed {};
    struct Scoped {};
    struct Zombified {};

    using StateType = std::variant<Managed, Scoped, Zombified>;

    Connection(Callback callback, StateType state, InputSignal* signal);

    Callback m_callback;
    StateType m_state;
    InputSignal* m_signal;
};

struct ScopedConnection
{
    friend class InputSignal;

    ScopedConnection();
    ScopedConnection(const ScopedConnection& other) = default;
    ScopedConnection(ScopedConnection&& other);
    ScopedConnection& operator=(const ScopedConnection& other) = default;
    ScopedConnection& operator=(ScopedConnection&& other);
    ~ScopedConnection();

private:
    explicit ScopedConnection(EventType eventType, Connection* connection)
            : m_eventType(eventType), m_connection(connection)
    {}

    void disconnect() const;
    mutable Connection* m_connection;
    mutable EventType m_eventType;
};

struct SlotConnection
{
    friend class InputSignal;

    void block();
    void unblock();
    auto scopedBlock();

    SlotConnection(InputSignal& signal, EventType eventType, const Connection::Callback& callback);
    SlotConnection(const SlotConnection& other) = delete;
    SlotConnection(SlotConnection&& other) = delete;
    SlotConnection& operator=(const SlotConnection& other) = delete;
    SlotConnection& operator=(SlotConnection&& other) = delete;

private:
    ScopedConnection m_scopedConnection;
    Semaphore m_semaphore;
};


#endif //POPOSIBENGINE_CONNECTION_HPP
