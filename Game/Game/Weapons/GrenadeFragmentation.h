//
// Created by 33644 on 02/03/2023.
//

#pragma once

#include "IWeapons.h"
#include "Engine/Core/Components/Physics.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
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

        void OnAwake() override;

        void Activate() override;
    };
}
