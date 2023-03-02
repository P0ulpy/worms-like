//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/ECS/Component/Component.hpp>
#include <Engine/Core/Time.hpp>

class GameStatesManager : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(GameStatesManager, Engine::Component)

    void OnAwake();
    void OnUpdate(Engine::Timestep dt);
};