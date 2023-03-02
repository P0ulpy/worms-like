//
// Created by Flo on 27/02/2023.
//

#pragma once

#include "Engine/Core/EntityPrefab/EntityPrefab.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "Engine/Core/Components/SpriteRenderer.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

class TestPrefab : public Engine::EntityPrefab<Engine::Components::Transform, Engine::Components::SpriteRenderer>
{
public:
    void Init(Engine::Entity& entity) override;
};

void TestPrefab::Init(Engine::Entity &entity)
{
    entity.GetComponent<Engine::Components::Transform>()->Pos = Engine::Components::Transform::PointT(0.f, 0.f);

    auto* krabTexture = Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/krab.png");
    entity.GetComponent<Engine::Components::SpriteRenderer>()->Init(krabTexture);
}
