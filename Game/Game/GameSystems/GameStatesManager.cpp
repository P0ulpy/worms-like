//
// Created by Flo on 28/02/2023.
//

#include "GameStatesManager.hpp"

GameStatesManager* GameStatesManager::s_instance = nullptr;

GameStatesManager *GameStatesManager::Get() {
    return s_instance;
}

void GameStatesManager::OnAwake()
{
    s_instance = this;

    m_playStates = GetEntity().GetComponent<PlayStates>();
    m_playStates->Init(2, 4);
}

void GameStatesManager::OnUpdate(Engine::Timestep dt)
{

}
