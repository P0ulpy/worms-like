//
// Created by Admin on 13/02/2023.
//

#ifndef POPOSIBENGINE_CREATURE_HPP
#define POPOSIBENGINE_CREATURE_HPP


#include "Engine/ECS/Entity/Entity.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Inputs/InputSignal.h"
#include <utility>

class Creature {
public:
    Creature()
        : m_moveUpConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerUp, this, &Creature::UpDirection))
        , m_moveDownConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerDown, this, &Creature::DownDirection))
        , m_moveLeftConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerLeft, this, &Creature::LeftDirection))
        , m_moveRightConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerRight, this, &Creature::RightDirection))
    {
        LockMovement();
    }
    explicit Creature(Engine::Entity& entity)
        : m_entity(entity)
        , m_moveUpConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerUp, this, &Creature::UpDirection))
        , m_moveDownConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerDown, this, &Creature::DownDirection))
        , m_moveLeftConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerLeft, this, &Creature::LeftDirection))
        , m_moveRightConnection(InputSignal::GetInstance()->connectSlot(EventType::PlayerRight, this, &Creature::RightDirection))
    {
        LockMovement();
    }

    void LockMovement() {
        m_moveUpConnection.block();
        m_moveDownConnection.block();
        m_moveLeftConnection.block();
        m_moveRightConnection.block();
    }

    void UnlockMovement() {
        m_moveUpConnection.unblock();
        m_moveDownConnection.unblock();
        m_moveLeftConnection.unblock();
        m_moveRightConnection.unblock();
    }

    void SetHealth(int health) { m_health = health; }
    [[nodiscard]] int GetHealth() const { return m_health; }

    void SetEntity(Engine::Entity& entity) { m_entity = entity; }
    [[nodiscard]] Engine::Entity GetEntity() const { return m_entity; }

    void SetTransform(Engine::Transform* transform) { m_transform = transform; }
    [[nodiscard]] Engine::Transform* GetTransform() const { return m_transform; }

    [[nodiscard]] bool IsDead() const { return m_health <= 0; }

    void UpDirection() {
        m_transform->Pos.y -= 5;
    }

    void DownDirection() {
        m_transform->Pos.y += 5;
    }

    void LeftDirection() {
        m_transform->Pos.x -= 5;
    }

    void RightDirection() {
        m_transform->Pos.x += 5;
    }

private:
    int m_health = 100;
    Engine::Entity m_entity;
    Engine::Transform* m_transform = nullptr;

    SlotConnection m_moveUpConnection;
    SlotConnection m_moveDownConnection;
    SlotConnection m_moveLeftConnection;
    SlotConnection m_moveRightConnection;
};


#endif //POPOSIBENGINE_CREATURE_HPP
