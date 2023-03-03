#include "PlayerCharacter.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

namespace Game::Actors
{
    void PlayerCharacter::OnAwake()
    {

    }

    void PlayerCharacter::Init(sf::Texture* texture)
    {
        m_sprite.setTexture(*texture);
        m_sprite.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

        SetStateMachine<CharacterMovementStateMachineGrounded>();
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
        Rect->BoundingBox.Height = m_sprite.getGlobalBounds().height;
        Rect->BoundingBox.Width = m_sprite.getGlobalBounds().width;
        RigidBodyComponent->SetRigidBody(Rect);
        RigidBodyComponent->CanRotate = false;

        RigidBodyComponent->SetOnCollisionCallback([this](Engine::Components::Physics::RigidBody2DdComponent* WithComponent, Maths::Vector2D<double> CollisionNormal)->bool {
            if (WithComponent->GetEntity().HasComponent<PlayerCharacter>()) return true;

            // @todo use line trace instead
            auto scalar = std::abs(CollisionNormal.Scalar(Engine::Components::Physics::GetUpVector2D<double>()));
            if (scalar >= 0.9f) IsGrounded = true;
            return false;
        });
    }

    void PlayerCharacter::OnDestroy() {
        delete StateMachine;
    }

    void PlayerCharacter::OnUpdate(const float &DeltaTime) {
        m_sprite.setPosition({static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetX()), static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetY())});
    }

    void PlayerCharacter::OnRender(sf::RenderTarget& renderTarget) {
        renderTarget.draw(m_sprite);
    }
}
