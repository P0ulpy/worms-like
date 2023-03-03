//
// Created by Flo on 28/02/2023.
//
#pragma once

#include <Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Time.hpp>
#include <memory>

#include "../Actors/PlayerCharacter.hpp"
#include "../../UI/Components/ProgressBar/HealthBar.hpp"

class Player : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(Player, Engine::Component)

    // This code is a placeholder to create a contract between the Player and game systems
    // it will be replaced by a real implementation later

    void OnAwake()
    {
        m_playerCharacter = GetEntity().GetComponent<Game::Actors::PlayerCharacter>();
    }

    void Init(const std::string& name, Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget);

    void OnPlayerTurnBegin();
    void OnUpdate(Engine::Timestep dt);
    void OnPlayerTurnEnd();
    void OnTakeDamage(float damage);

    void OnDestroy()
    {
        OnDeath();
    }

    [[nodiscard]] float GetHealth() const { return m_health; }

    bool dead = false;

private:
    void OnDeath();

private:
    Game::Actors::PlayerCharacter* m_playerCharacter = nullptr;
    Game::UI::HealthBar* progressBarHealthWidget = nullptr;

    std::string m_name {};
    float m_health = 100.f;
};
