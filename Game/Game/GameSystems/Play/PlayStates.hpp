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
#include "../../Controllers/SidePlayerController.hpp"

constexpr float RoundsDuration = 10000.f;

class PlayStates : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayStates, Engine::Component);

    void OnAwake() { }

    void Init(int teamCount, int teamsSize)
    {
        CreatePlayer();
        CreateUI();

        m_teams.reserve(teamCount);

        static const std::array<sf::Texture*, 3> playersTextures = {
                Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Player/p1_stand.png"),
                Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Player/p2_stand.png"),
                Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Player/p3_stand.png"),
        };

        for(int i = 0; i < teamCount; i++)
        {
            auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
            auto* newTeam = scene->CreateEntity().AddComponent<PlayerTeam>();

            auto* texture = i + 1 >= playersTextures.size() ? playersTextures[0] : playersTextures[i];
            newTeam->Init("Team " + std::to_string(i), teamsSize, verticalBoxPlayersWidget, texture, i * 100.f);

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
    [[nodiscard]] Game::Controllers::SidePlayerController* GetPlayerController() const { return m_playerController; }

private:

    void CreatePlayer();
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

    Game::Controllers::SidePlayerController* m_playerController { nullptr };

    SignalSystem::ScopedConnectionSignal m_inventoryConnection;

    sf::Clock gameClock;

    float m_roundTimer = RoundsDuration;

    int m_currentTeamIndex = 0;
    std::vector<PlayerTeam*> m_teams;
};
