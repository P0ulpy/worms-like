//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/Core/EntityPrefab/EntityPrefab.hpp>
#include <Engine/Core/Components/Transform.hpp>
#include "../../../Engine/Engine/Core/EntityPrefab/EntityPrefab.hpp"

#include "Player.hpp"
#include "PlayerController.hpp"

class PlayerPrefab : public Engine::EntityPrefab<Player, PlayerController>
{
public:
    void Init(Engine::Entity& entity) override
    {

    }
};