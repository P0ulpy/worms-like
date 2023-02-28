//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/Core/EntityPrefab/EntityPrefab.hpp>
#include <Engine/Core/Components/Transform.hpp>

#include "Player.hpp"
#include "PlayerController.hpp"

class PlayerPrefab : public Engine::EntityPrefab<Engine::Transform, Player, PlayerController>
{
public:
    void Init(Engine::Entity& entity) override
    {

    }
};