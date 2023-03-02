//
// Created by Flo on 28/02/2023.
//
#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Time.hpp>

#include "PlayerController.hpp"

class Player : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(Player, Engine::Component)

    // This code is a placeholder to create a contract between the Player and game systems
    // it will be replaced by a real implementation later

    void OnAwake()
    {
        m_playerController = GetEntity().GetComponent<PlayerController>();
    }

    void Init(const std::string_view& name);

    void OnPlayerTurnBegin();
    void OnUpdate(Engine::Timestep dt);
    void OnPlayerTurnEnd();
    void OnTakeDamage(float damage);

    [[nodiscard]] float GetHealth() const { return m_health; }

private:

    void OnDeath();

private:
    PlayerController* m_playerController = nullptr;

private:
    std::string_view m_name {};
    float m_health = 100.f;
};
