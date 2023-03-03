//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/Core/EntityPrefab/EntityPrefab.hpp>

#include "GameStatesManager.hpp"
#include "Play/PlayStates.hpp"

class GameManagerPrefab : public Engine::EntityPrefab<GameStatesManager, PlayStates>
{
public:

    void Init(Engine::Entity& entity) override
    {

    }


};