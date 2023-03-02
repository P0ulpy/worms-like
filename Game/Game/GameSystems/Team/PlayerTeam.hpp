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
#include <Engine/UI/Layout/Plan/WidgetPlan.hpp>
#include <Engine/UI/Components/Text/TextWidget.hpp>
#include <Engine/UI/Components/Sprite/SpriteWidget.hpp>
#include <Engine/AssetLoader/AssetLoader.hpp>

#include "../../Player/Player.hpp"
#include "../../Player/PlayerPrefab.hpp"
#include "TeamUI.hpp"

#include "../../../../Engine/Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/AssetLoader/AssetLoader.hpp"
#include "../../../../Engine/Engine/UI/Components/Text/TextWidget.hpp"
#include "../../../../Engine/Engine/UI/Components/Sprite/SpriteWidget.hpp"
#include "../../../../Engine/Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"

class PlayerTeam : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayerTeam, Engine::Component);

    void OnAwake()
    {

    }

    void Init(const std::string_view& name, int teamSize, TeamUI* teamUI)
    {
        m_teamUI = teamUI;
        m_teamName = name;
        m_players.reserve(teamSize);

        for(int i = 0; i < teamSize; i++)
        {
            auto newPlayer = Engine::SceneSystem::Get()->GetActiveScene()
                    ->InstantiatePrefab<PlayerPrefab>();

            auto* player = newPlayer->GetComponent<Player>();
            player->Init(std::string(m_teamName) + " | Player " + std::to_string(i));
            m_players.push_back(player);
        }
    }

    void OnTeamTurnStart()
    {
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
    TeamUI* m_teamUI { nullptr };

    std::string_view m_teamName {};

    int m_currentPlayerIndex = 0;
    std::vector<Player*> m_players;
};