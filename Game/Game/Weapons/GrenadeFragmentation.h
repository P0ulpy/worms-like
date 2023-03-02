//
// Created by 33644 on 02/03/2023.
//

#pragma once

#include "IWeapons.h"
#include "Engine/Core/Components/Physics.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "../Prefabs/GrenadePrefab.h"
#include "Engine/Core/Components/Physics.hpp"

namespace Game::Weapons
{
    class GrenadeFragmentation : public Weapons
    {
    public:
        DECLARE_CLASS_TYPE(GrenadeFragmentation, Weapons)

        using RequiredComponents = Engine::ComponentRequirements<Engine::Components::Physics::RigidBody2DdComponent>;

        GrenadeFragmentation() = default;

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
             std::array<Maths::Vector2D<double>, 5> directions = {
                    Maths::Vector2D<double>(-1.5f, -2.5f),
                    Maths::Vector2D<double>(1.5f, -2.5f),
                    Maths::Vector2D<double>(-1.5f, -3.f),
                    Maths::Vector2D<double>(1.5f, -3.f),
                    Maths::Vector2D<double>(0.f, -3.5f)
                };

            for(int i = 0; i < 5; i++)
            {
                auto prefab = GetScene()->InstantiatePrefab<GrenadePrefab>();
                prefab.GetComponent<Engine::Components::Physics::RigidBody2DdComponent>()->GetRigidBody()->LinearVelocity =
                        prefab.GetComponent<Engine::Components::Physics::RigidBody2DdComponent>()->GetRigidBody()->LinearVelocity
                        + directions[i];

                prefab.GetComponent<Engine::Components::Transform>()->Pos = GetEntity().GetComponent<Engine::Components::Transform>()->Pos - Maths::Vector2D<double>(0.f, m_sprite.getGlobalBounds().height / 2);
            }

            GetScene()->DestroyEntity(GetEntity());
        }


    };
}
