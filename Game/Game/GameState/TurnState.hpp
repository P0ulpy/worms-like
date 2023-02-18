//
// Created by Admin on 13/02/2023.
//

#ifndef POPOSIBENGINE_TURNSTATE_HPP
#define POPOSIBENGINE_TURNSTATE_HPP

enum class TurnState
{
    PlayerOne,
    PlayerTwo,
    Other,
    EndTurn
};

class TurnGameState
{
public:

    TurnGameState() = default;
    ~TurnGameState() = default;

    [[nodiscard]] TurnState GetCurrentTurnState() const { return m_CurrentTurnState; }

    void ChangeTurnState();


private:
    TurnState m_lastPlayerPlayed = TurnState::PlayerOne;
    TurnState m_CurrentTurnState = TurnState::PlayerOne;
};

#endif //POPOSIBENGINE_TURNSTATE_HPP
