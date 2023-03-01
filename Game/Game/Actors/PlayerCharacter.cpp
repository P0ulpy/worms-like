#include "PlayerCharacter.hpp"

namespace Game::Actors {
    void PlayerCharacter::OnAwake() {
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        auto Rect = new Engine::Components::Physics::RectangleRigidBody2Dd();
        Rect->BoundingBox.Height = 150.f;
        Rect->BoundingBox.Width = 50.f;
        RigidBodyComponent->SetRigidBody(Rect);
        RigidBodyComponent->CanRotate = false;

        m_KeyJumpConnection = SignalSystem::InputSignal::Get()->connectScoped("player_jump", [this]()->void {StateMachine->OnJump();});
        m_KeyDownConnection = SignalSystem::InputSignal::Get()->connectScoped("player_down", [this]()->void {StateMachine->OnDown();});
        m_KeyLeftConnection = SignalSystem::InputSignal::Get()->connectScoped("player_left", [this]()->void {StateMachine->OnLeft();});
        m_KeyRightConnection = SignalSystem::InputSignal::Get()->connectScoped("player_right", [this]()->void {StateMachine->OnRight();});
    }

    PlayerCharacter::PlayerCharacter() {
        SetStateMachine<CharacterMovementStateMachineGrounded>();
    }

    void PlayerCharacter::OnJump() {
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity = RigidBodyComponent->GetRigidBody()->LinearVelocity + Maths::Vector2D<double>(0.f, -20.f);
        // @todo make grounded again
//        SetStateMachine<CharacterMovementStateMachineJumping>();
    }

    void PlayerCharacter::OnLeft() {
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity = RigidBodyComponent->GetRigidBody()->LinearVelocity + Maths::Vector2D<double>(-20.f, 0.f);
    }

    void PlayerCharacter::OnRight() {
        auto* RigidBodyComponent = GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity = RigidBodyComponent->GetRigidBody()->LinearVelocity + Maths::Vector2D<double>(20.f, 0.f);
    }

    void PlayerCharacter::OnDown() {

    }

    PlayerCharacter::~PlayerCharacter() {
        delete StateMachine;
    }
}
