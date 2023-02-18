//
// Created by Admin on 13/02/2023.
//

#include "Player.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/Core/Components/SpriteRenderer.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

Player::Player(const std::string &textureName)
{
    for(auto & m_creature : m_creatures) {
        auto entity = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity();

        auto transform = entity.AddComponent<Engine::Transform>();
        transform->Scale = { 0.5f, 0.5f };

        auto sprite = entity.AddComponent<Engine::SpriteRenderer>();
        sprite->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset(textureName));
        sprite->transform = transform;

        m_creature.SetEntity(entity);
        m_creature.SetTransform(transform);
    }
}
