//
// Created by Flo on 02/03/2023.
//

#pragma once

#include <vector>
#include <string_view>
#include <string>

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/ECS/Scene/Scene.hpp>
#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Time.hpp>
#include <Engine/Core/ScenesSystem/ScenesSystem.hpp>
#include <Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp>
#include <Engine/UI/Layout/Plan/WidgetPlan.hpp>
#include <Engine/UI/Components/Text/TextWidget.hpp>
#include <Engine/UI/Components/Sprite/SpriteWidget.hpp>
#include <Engine/AssetLoader/AssetLoader.hpp>

#include "../../Player/Player.hpp"
#include "../../Player/PlayerPrefab.hpp"
#include "../../../UI/Components/ProgressBar/HealthBar.hpp"

class PlayerTeam : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayerTeam, Engine::Component);

    void OnAwake()
    {

    }

    void Init(const std::string& name, int teamSize, Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget)
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

        m_teamName = name;
        m_players.reserve(teamSize);

        for(int i = 0; i < teamSize; i++)
        {
            auto PlayerCharacterEntity = scene->CreateEntity();

            auto* playerCharacter = PlayerCharacterEntity.AddComponent<Game::Actors::PlayerCharacter>();
            auto* player = PlayerCharacterEntity.AddComponent<Player>();
            PlayerCharacterEntity.GetComponent<Engine::Components::Transform>()
                ->Pos = Maths::Point2D<double>((500.f * i) + 300.f, (double) (30.f * 30.f * -1));

            player->Init({ m_teamName + " | Player " + std::to_string(i) }, verticalBoxPlayersWidget);

            m_players.push_back(player);
        }
    }

    void OnTeamTurnStart()
    {
        Engine::Logger::Log("Team ", std::string(m_teamName), " turn start");

        GetCurrentPlayer()->OnPlayerTurnBegin();
    }

    void OnTeamTurnEnd()
    {
        GetCurrentPlayer()->OnPlayerTurnEnd();
        IncrementToNextPlayer();
    }

    Player* GetCurrentPlayer()
    {
        return m_players[m_currentPlayerIndex];
    }

private:

    void IncrementToNextPlayer()
    {
        ++m_currentPlayerIndex;
        if(m_currentPlayerIndex >= m_players.size())
            m_currentPlayerIndex = 0;
    }

private:
    std::string m_teamName {};

    int m_currentPlayerIndex = 0;
    std::vector<Player*> m_players;
};