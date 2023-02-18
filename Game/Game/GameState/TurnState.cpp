//
// Created by Admin on 13/02/2023.
//

#include "TurnState.hpp"

void TurnGameState::ChangeTurnState() {
    switch (m_CurrentTurnState)
    {
        case TurnState::PlayerOne:
            m_lastPlayerPlayed = TurnState::PlayerOne;
            m_CurrentTurnState = TurnState::EndTurn;
            break;

        case TurnState::PlayerTwo:
            m_lastPlayerPlayed = TurnState::PlayerTwo;
            m_CurrentTurnState = TurnState::EndTurn;
            break;

        case TurnState::Other:
            if(m_lastPlayerPlayed == TurnState::PlayerOne)
                m_CurrentTurnState = TurnState::PlayerTwo;
            else
                m_CurrentTurnState = TurnState::PlayerOne;
            break;

        case TurnState::EndTurn:
            m_CurrentTurnState = TurnState::Other;
            break;
    }
}
