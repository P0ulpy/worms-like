//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Time.hpp>
#include "Engine/Core/Inputs/Connection.hpp"
#include "Play/PlayStates.hpp"

class GameStatesManager : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(GameStatesManager, Engine::Component)

    void OnAwake();
    void OnUpdate(Engine::Timestep dt);

private:
    void CreateUI();

private:
    PlayStates* m_playStates = nullptr;

    SignalSystem::ScopedConnectionSignal m_inventoryConnection;
};