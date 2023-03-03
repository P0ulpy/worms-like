#include "GrenadeFragmentation.h"
#include "Grenade.h"
#include "../Prefabs/GrenadePrefab.h"
#include "../Actors/PlayerCharacter.hpp"
#include "../Player/Player.hpp"

namespace Game::Weapons {
    void GrenadeFragmentation::OnAwake()
    {
        m_damage = 10.f;

        auto texture = Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Items/bomb.png");
        m_sprite.setTexture(*texture);
        m_sprite.setScale(0.5f, 0.5f);
        m_sprite.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
        RigidBodyComponent->SetOnCollisionCallback([this](Engine::Components::Physics::RigidBody2DdComponent* WithComponent, const Maths::Vector2D<double>& CollisionNormal)->bool {
            auto Entity = WithComponent->GetEntity();
            if (
                Entity.HasComponent<GrenadeFragmentation>()
                || Entity.HasComponent<Grenade>()
            )
            {
                return true;
            }

            if (Entity.HasComponent<Actors::PlayerCharacter>())
            {
                if (Entity.GetComponent<Actors::PlayerCharacter>() == Owner) return true;
                Entity.GetComponent<Player>()->OnTakeDamage(5.f);
            }
            Activate();
            return false;
        });
        Rect->BoundingBox.Height = m_sprite.getGlobalBounds().height;
        Rect->BoundingBox.Width = m_sprite.getGlobalBounds().width;
        RigidBodyComponent->SetRigidBody(Rect);
    }

    void GrenadeFragmentation::Activate()
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
}