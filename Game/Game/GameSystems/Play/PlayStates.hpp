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
#include "TeamsTimersUI.hpp"

constexpr float RoundsDuration = 10000.f;

class PlayStates : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayStates, Engine::Component);

    void OnAwake()
    {

    }

    void Init(int teamCount, int teamsSize, Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget)
    {
        m_timerUI = std::make_unique<TimerUI>(verticalBoxPlayersWidget);

        m_teams.reserve(teamCount);

        for(int i = 0; i < teamCount; i++)
        {
            auto newTeamEntity = Engine::ScenesSystem::Get()->GetActiveScene()->CreateEntity();
            auto* newTeam = newTeamEntity.AddComponent<PlayerTeam>();
            newTeam->Init("Team " + std::to_string(i), teamsSize, m_timerUI.get());

            m_teams.push_back(newTeam);
        }

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
    }

    void OnUpdate(Engine::Timestep dt)
    {
        if(m_roundTimer < RoundsDuration)
            OnTurnUpdate(dt);
    }

    void OnTurnUpdate(Engine::Timestep dt)
    {
        m_roundTimer += dt;

        m_timerUI->textTimerTurnValueWidget->SetText(std::to_string((int)(m_roundTimer / 1000)));

        if(m_roundTimer >= RoundsDuration)
            OnTurnEnd();
    }

    [[nodiscard]] PlayerTeam* GetCurrentTeam() const { return m_teams[m_currentTeamIndex]; }

private:

    void IncrementToNextTeam()
    {
        ++m_currentTeamIndex;
        if(m_currentTeamIndex >= m_teams.size())
            m_currentTeamIndex = 0;
    }

    std::unique_ptr<TimerUI> m_timerUI { nullptr };

    float m_roundTimer = RoundsDuration;

    int m_currentTeamIndex = 0;
    std::vector<PlayerTeam*> m_teams;
};