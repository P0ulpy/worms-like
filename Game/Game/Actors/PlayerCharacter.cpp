#include "PlayerCharacter.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

namespace Game::Actors {
    void PlayerCharacter::OnAwake() {
        auto texture = Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Player/p1_stand.png");
        m_sprite.setTexture(*texture);
        m_sprite.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

        SetStateMachine<CharacterMovementStateMachineGrounded>();
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
        Rect->BoundingBox.Height = m_sprite.getGlobalBounds().height;
        Rect->BoundingBox.Width = m_sprite.getGlobalBounds().width;
        RigidBodyComponent->SetRigidBody(Rect);
        RigidBodyComponent->CanRotate = false;

//        RigidBodyComponent->SetOnCollisionCallback([this]() {
//
//        });
    }

    PlayerCharacter::~PlayerCharacter() {
        delete StateMachine;
    }

    void PlayerCharacter::OnUpdate(const float &DeltaTime) {
        m_sprite.setPosition({static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetX()), static_cast<float>(GetEntity().GetComponent<Engine::Components::Transform>()->Pos.GetY())});
    }

    void PlayerCharacter::OnRender(sf::RenderTarget& renderTarget) {
        renderTarget.draw(m_sprite);
    }
}
