//
// Created by Flo on 27/02/2023.
//

#pragma once

#include "../../../Engine/Engine/Core/EntityPrefab/EntityPrefab.hpp"
#include "../../../Engine/Engine/Core/Components/Transform.hpp"
#include "../../../Engine/Engine/Core/Components/SpriteRenderer.hpp"
#include "../../../Engine/Engine/AssetLoader/AssetLoader.hpp"

class TestPrefab : public Engine::EntityPrefab<Engine::Transform, Engine::SpriteRenderer>
{
public:
    void Init(Engine::Entity& entity) override;
};

void TestPrefab::Init(Engine::Entity &entity)
{
    entity.GetComponent<Engine::Transform>()->Pos = { 0, 0 };

    auto* krabTexture = Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/krab.png");
    entity.GetComponent<Engine::SpriteRenderer>()->Init(krabTexture);
}
