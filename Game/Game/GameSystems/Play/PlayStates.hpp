//
// Created by Flo on 02/03/2023.
//

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/ScenesSystem/ScenesSystem.hpp>
#include <Engine/ECS/Scene/Scene.hpp>
#include <Engine/Core/Time.hpp>

#include <vector>

#include "../../Player/Player.hpp"
#include "../../Player/PlayerPrefab.hpp"
#include "../Team/PlayerTeam.hpp"
#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"

constexpr float RoundsDuration = 10000.f;

class PlayStates : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayStates, Engine::Component);

    void OnAwake() { }

    void Init(int teamCount, int teamsSize)
    {
        CreateUI();

        m_teams.reserve(teamCount);

        for(int i = 0; i < teamCount; i++)
        {
            auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
            auto* newTeam = scene->CreateEntity().AddComponent<PlayerTeam>();
            newTeam->Init("Team " + std::to_string(i), teamsSize, verticalBoxPlayersWidget);

            m_teams.push_back(newTeam);
        }

        gameClock.restart();
        OnTurnStart();
    }

    void OnTurnStart()
    {
        m_roundTimer = .0f;
        GetCurrentTeam()->OnTeamTurnStart();
    }

    void OnTurnEnd()
    {
        GetCurrentTeam()->OnTeamTurnEnd();
        IncrementToNextTeam();
        OnTurnStart();
    }

    void OnUpdate(Engine::Timestep dt)
    {
        auto time = gameClock.getElapsedTime();
        std::stringstream timeStream;
        timeStream
            << (int)((time.asSeconds() / 60) / 60) << ':'
            << (int)(time.asSeconds() / 60) << ':'
            << (int)time.asSeconds();

        textTimerGlobalWidget->SetText(timeStream.str());

        if(m_roundTimer < RoundsDuration)
            OnTurnUpdate(dt);
    }

    void OnTurnUpdate(Engine::Timestep dt)
    {
        m_roundTimer += dt;

        textTimerTurnValueWidget->SetText(std::to_string((int)(m_roundTimer / 1000)));

        if(m_roundTimer >= RoundsDuration)
            OnTurnEnd();
    }

    [[nodiscard]] PlayerTeam* GetCurrentTeam() const { return m_teams[m_currentTeamIndex]; }

private:

    void CreateUI();

    void IncrementToNextTeam()
    {
        ++m_currentTeamIndex;
        if(m_currentTeamIndex >= m_teams.size())
            m_currentTeamIndex = 0;
    }

private:
    Engine::UI::TextWidget* textTimerGlobalWidget { nullptr };
    Engine::UI::TextWidget* textTimerTurnValueWidget { nullptr };
    Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget { nullptr };

    SignalSystem::ScopedConnectionSignal m_inventoryConnection;

    sf::Clock gameClock;

    float m_roundTimer = RoundsDuration;

    int m_currentTeamIndex = 0;
    std::vector<PlayerTeam*> m_teams;
};
