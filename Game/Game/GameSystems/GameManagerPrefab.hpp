//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/Core/EntityPrefab/EntityPrefab.hpp>

#include "GameStatesManager.hpp"

class GameManagerPrefab : public Engine::EntityPrefab<GameStatesManager>
{
public:

    void Init(Engine::Entity& entity) override
    {

    }
};