//
// Created by Admin on 12/02/2023.
//

#ifndef POPOSIBENGINE_GAME_HPP
#define POPOSIBENGINE_GAME_HPP


#include "Engine/Core/Application/ApplicationLayer.hpp"
#include "Player/Player.hpp"
#include "GameState/TurnState.hpp"

class GameLayer : public Engine::ApplicationLayer
{
public:
    GameLayer()
            : Engine::ApplicationLayer("GameLayer")
    {}

    ~GameLayer() override
    {
        delete player;
        delete player2;
    }

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

private:
    Player* player = nullptr;
    Player* player2 = nullptr;

    Creature* selectedCreature = nullptr;

    TurnGameState m_TurnGameState;
    float m_TurnTimer = 0.0f;
    float m_TurnPlayerDuration = 5.0f;
    float m_OtherTurnDuration = 2.5f;
};


#endif //POPOSIBENGINE_GAME_HPP
