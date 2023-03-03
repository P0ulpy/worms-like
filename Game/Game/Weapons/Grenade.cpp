#include "Grenade.h"
#include "GrenadeFragmentation.h"
#include "../Actors/PlayerCharacter.hpp"
#include "../Player/Player.hpp"

namespace Game::Weapons {
    void Grenade::OnAwake()
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
}