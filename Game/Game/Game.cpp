//
// Created by Admin on 12/02/2023.
//

#include "Game.hpp"

#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "GameState/GameState.hpp"

void GameLayer::OnAttach()
{
    player = new Player("../../Assets/p1_front.png");

    for(int i = 0; i < player->GetCreatures().size(); i++)
        player->GetCreatures()[i].GetEntity().GetComponent<Engine::Transform>()->Pos = { 100.f * i + 50.0f, 100.f * i + 50.0f };

    player2 = new Player("../../Assets/p2_front.png");

    for(int i = 0; i < player2->GetCreatures().size(); i++)
        player2->GetCreatures()[i].GetEntity().GetComponent<Engine::Transform>()->Pos = { 100.f * i + 50.0f, 100.f * i + 50.0f + 100.f };

    selectedCreature = player->GetPlayableCreature();
    selectedCreature->UnlockMovement();
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Engine::Timestep ts)
{
    m_TurnTimer += ts;

    if(m_TurnGameState.GetCurrentTurnState() == TurnState::PlayerOne || m_TurnGameState.GetCurrentTurnState() == TurnState::PlayerTwo)
    {
        if(m_TurnTimer >= m_TurnPlayerDuration)
        {
            m_TurnGameState.ChangeTurnState();
            m_TurnTimer = 0.0f;

            selectedCreature->LockMovement();
            selectedCreature = nullptr;
        }
    }
    else if(m_TurnGameState.GetCurrentTurnState() == TurnState::Other)
    {
        if(m_TurnTimer >= m_OtherTurnDuration)
        {
            m_TurnGameState.ChangeTurnState();
            m_TurnTimer = 0.0f;

            if(m_TurnGameState.GetCurrentTurnState() == TurnState::PlayerOne && player != nullptr)
                selectedCreature = player->GetPlayableCreature();
            else if(m_TurnGameState.GetCurrentTurnState() == TurnState::PlayerTwo && player2 != nullptr)
                selectedCreature = player2->GetPlayableCreature();

            if(selectedCreature != nullptr)
                selectedCreature->UnlockMovement();
        }
    }
    else if(m_TurnGameState.GetCurrentTurnState() == TurnState::EndTurn)
    {
        if(m_TurnTimer >= m_OtherTurnDuration)
        {
            m_TurnGameState.ChangeTurnState();
            m_TurnTimer = 0.0f;
        }
    }
}

void GameLayer::OnImGuiRender()
{
    ApplicationLayer::OnImGuiRender();
}
