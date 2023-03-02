//
// Created by 33644 on 02/03/2023.
//

#pragma once

#include "Engine/Core/EntityPrefab/EntityPrefab.hpp"
#include "../Weapons/GrenadeFragmentation.h"

class GrenadeFragPrefab : public Engine::EntityPrefab<Game::Weapons::GrenadeFragmentation>
{
public:
    void Init(Engine::Entity& entity) override;
};

void GrenadeFragPrefab::Init(Engine::Entity &entity)
{
    auto grenade = entity.AddComponent<Game::Weapons::GrenadeFragmentation>();
    auto grenadeTransform = entity.GetComponent<Engine::Components::Transform>();

    const auto& Window = Engine::EngineApplication::Get()->GetWindow();
    auto MousePixelPosition = sf::Mouse::getPosition();
    auto MousePos = Window.mapPixelToCoords(MousePixelPosition);
    grenadeTransform->Pos = Maths::Point2D<double>(static_cast<double>(MousePos.x), static_cast<double>(MousePos.y));
}