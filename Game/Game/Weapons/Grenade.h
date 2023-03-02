//
// Created by 33644 on 02/03/2023.
//

#pragma once

#include "IWeapons.h"
#include "Engine/Core/Components/Physics.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

namespace Game::Weapons
{
    class Grenade : public Weapons
    {
    public:
        DECLARE_CLASS_TYPE(Grenade, Weapons)

        using RequiredComponents = Engine::ComponentRequirements<Engine::Components::Physics::RigidBody2DdComponent>;

        Grenade() = default;

        void OnUpdate(const float& DeltaTime) override
        {
            m_sprite.setPosition({static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetX()), static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetY())});
        }

        void OnAwake() override
        {
            m_damage = 10.f;

            auto texture = Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Items/bomb.png");
            m_sprite.setTexture(*texture);
            m_sprite.setScale(0.5f, 0.5f);
            m_sprite.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

            auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
            auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBodyComponent->SetOnCollisionCallback([this]() { Activate(); });
            Rect->BoundingBox.Height = m_sprite.getGlobalBounds().height;
            Rect->BoundingBox.Width = m_sprite.getGlobalBounds().width;
            RigidBodyComponent->SetRigidBody(Rect);

            RigidBodyComponent->isGrenade = true; //@TODO: remove this
        }

        void Activate() override
        {
            m_sprite.setColor(sf::Color::Red);
            GetScene()->DestroyEntity(GetEntity());
        };
    };
}
