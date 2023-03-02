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

        void OnAwake() override;

        void Activate() override
        {
            m_sprite.setColor(sf::Color::Red);
            GetScene()->DestroyEntity(GetEntity());
        };
    };
}
