//
// Created by Flo on 28/02/2023.
//

#include "GameStatesManager.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"

void GameStatesManager::OnAwake()
{
    m_playStates = GetEntity().GetComponent<PlayStates>();
    m_playStates->Init(2, 4);
}

void GameStatesManager::OnUpdate(Engine::Timestep dt)
{

}
