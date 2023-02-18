//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_GAMESTATE_HPP
#define POPOSIBENGINE_GAMESTATE_HPP

#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Inputs/InputSignal.h"

enum class EGameState
{
    MainMenu,
    Game,
    Pause,
    EndGame
};



class GameState {
public:
    ~GameState() = default;

    static GameState* GetInstance()
    {
        if(nullptr == instance)
        {
            instance = new GameState();
        }

        return instance;
    }

    [[nodiscard]] EGameState GetState() const { return m_CurrentGameState; }
    void ChangeState(EGameState newGameState) { m_CurrentGameState = newGameState; }

    void SetPauseOrUnpause()
    {
        if(m_CurrentGameState == EGameState::MainMenu || m_CurrentGameState == EGameState::EndGame)
            return;

        if(m_CurrentGameState == EGameState::Game)
        {
            m_CurrentGameState = EGameState::Pause;
        }
        else if(m_CurrentGameState == EGameState::Pause)
        {
            m_CurrentGameState = EGameState::Game;
        }
    }


private:
    GameState()
    : m_PauseConnection(InputSignal::GetInstance()->connectSlot(EventType::Pause, this, &GameState::SetPauseOrUnpause))
    {}
    static GameState* instance;

    EGameState m_CurrentGameState = EGameState::MainMenu;
    SlotConnection m_PauseConnection;
};


#endif //POPOSIBENGINE_GAMESTATE_HPP
