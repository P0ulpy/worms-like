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
    static GameStatesManager* Get();
private:
    static GameStatesManager* s_instance;

public:
    DECLARE_CLASS_TYPE(GameStatesManager, Engine::Component)

    void OnAwake();
    void OnUpdate(Engine::Timestep dt);

    PlayStates* GetPlayStates() const { return m_playStates; }

private:
    PlayStates* m_playStates = nullptr;

    SignalSystem::ScopedConnectionSignal m_inventoryConnection;
};