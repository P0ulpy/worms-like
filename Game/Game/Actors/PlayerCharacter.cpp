#include "PlayerCharacter.hpp"

namespace Game::Actors {
    void PlayerCharacter::OnAwake() {
        SetStateMachine<CharacterMovementStateMachineGrounded>();
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
        Rect->BoundingBox.Height = 150.f;
        Rect->BoundingBox.Width = 50.f;
        RigidBodyComponent->SetRigidBody(Rect);
        RigidBodyComponent->CanRotate = false;
    }

    PlayerCharacter::~PlayerCharacter() {
        delete StateMachine;
    }
}
