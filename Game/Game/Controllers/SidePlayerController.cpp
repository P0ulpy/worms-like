#include "SidePlayerController.hpp"

namespace Game::Controllers
{
    void SidePlayerController::OnUpdate(const float& DeltaTime)
    {
        HandleCameraMovementFromMousePos(DeltaTime);

        if (nullptr != m_PlayerCharacter) {
            if (m_PlayerCharacter->IsGrounded)
                SetStateMachineOnPlayer<Actors::CharacterMovementStateMachineGrounded>();
            else
                SetStateMachineOnPlayer<Actors::CharacterMovementStateMachineInAir>();
        }
    }

    void SidePlayerController::HandleCameraMovementFromMousePos(const float& DeltaTime)
    {
        const auto& Window = Engine::EngineApplication::Get()->GetWindow();
        auto MousePixelPosition = sf::Mouse::getPosition();
        auto MousePos = Window.mapPixelToCoords(MousePixelPosition);

        auto *Camera = GetScene()->GetActiveCamera();
        auto *CastedCamera = dynamic_cast<Engine::Camera::Camera2D<double> *>(Camera);
        if (nullptr == CastedCamera) return;

        auto CameraSize = CastedCamera->GetCurrentSize();
        auto HalfCameraSize = CameraSize / 2;
        if (HalfCameraSize.GetLength() == 0) return;

        auto CenterToMousePos = CastedCamera->Position.GetVectorTo(Maths::Point2D<double>((double) MousePos.x, (double) MousePos.y));
        auto CenterToMousePosDirection = CenterToMousePos.GetNormalized();
        CenterToMousePos.GetX() = std::abs(CenterToMousePos.GetX());
        CenterToMousePos.GetY() = std::abs(CenterToMousePos.GetY());
        auto MinX = HalfCameraSize.GetX() * MinRatioToMoveCamera;
        auto MinY = HalfCameraSize.GetY() * MinRatioToMoveCamera;
        if (CenterToMousePos.GetX() <= MinX) {
            CenterToMousePos.GetX() = 0;
        } else {
            auto MaxX = HalfCameraSize.GetX() * MaxRatioToMoveCamera;
            CenterToMousePos.GetX() = std::min(MaxX, CenterToMousePos.GetX()) / MaxX;
        }
        if (CenterToMousePos.GetY() <= MinY) {
            CenterToMousePos.GetY() = 0;
        } else {
            auto MaxY = HalfCameraSize.GetY() * MaxRatioToMoveCamera;
            CenterToMousePos.GetY() = std::min(MaxY, CenterToMousePos.GetY()) / MaxY;   
        }
        CenterToMousePos = CenterToMousePos * CenterToMousePosDirection;

        CastedCamera->Position = CastedCamera->Position + Maths::Vector2D<double>(
            CenterToMousePos.GetX() * (CameraXRatioSpeed * CameraSize.GetX() * 1 / DeltaTime),
            CenterToMousePos.GetY() * (CameraYRatioSpeed * CameraSize.GetY() * 1 / DeltaTime)
        );

        // cap to min/max pos
        CastedCamera->Position.GetX() = std::max(std::min(CastedCamera->Position.GetX(), MaxPosition.GetX()), MinPosition.GetX());
        CastedCamera->Position.GetY() = std::max(std::min(CastedCamera->Position.GetY(), MaxPosition.GetY()), MinPosition.GetY());
    }

    void SidePlayerController::OnAwake() {
        SetWeaponToShoot([]() {});
        m_KeyJumpConnection = SignalSystem::InputSignal::Get()->connectScoped("player_jump", [this]()->void {if (nullptr != m_PlayerCharacter) m_PlayerCharacter->StateMachine->HandleJump();});
        m_KeyDownConnection = SignalSystem::InputSignal::Get()->connectScoped("player_down", [this]()->void {if (nullptr != m_PlayerCharacter) m_PlayerCharacter->StateMachine->HandleDown();});
        m_KeyLeftConnection = SignalSystem::InputSignal::Get()->connectScoped("player_left", [this]()->void {if (nullptr != m_PlayerCharacter) m_PlayerCharacter->StateMachine->HandleLeft();});
        m_KeyRightConnection = SignalSystem::InputSignal::Get()->connectScoped("player_right", [this]()->void {if (nullptr != m_PlayerCharacter) m_PlayerCharacter->StateMachine->HandleRight();});
        m_shootConnection = SignalSystem::InputSignal::Get()->connectScoped("player_shoot", [this]() {if (nullptr != m_PlayerCharacter) m_PlayerCharacter->StateMachine->HandleShoot();});
    }

    void SidePlayerController::OnJump() {
        if (nullptr == m_PlayerCharacter) return;
        auto* RigidBodyComponent = m_PlayerCharacter->GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity =
            RigidBodyComponent->GetRigidBody()->LinearVelocity
            + Maths::Vector2D<double>(0.f, (Engine::Components::Physics::RigidBody2DdComponent::Gravity / 2 + 3) * -1 );
        SetStateMachineOnPlayer<Actors::CharacterMovementStateMachineInAir>();
        m_PlayerCharacter->IsGrounded = false;
    }

    void SidePlayerController::OnLeft() {
        if (nullptr == m_PlayerCharacter) return;
        auto* RigidBodyComponent = m_PlayerCharacter->GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity = Maths::Vector2D<double>(-1.f, 0.f);
    }

    void SidePlayerController::OnRight() {
        if (nullptr == m_PlayerCharacter) return;
        auto* RigidBodyComponent = m_PlayerCharacter->GetEntity().GetComponent<Engine::Components::Physics::RigidBody2DdComponent>();
        RigidBodyComponent->GetRigidBody()->LinearVelocity = Maths::Vector2D<double>(1.f, 0.f);
    }

    void SidePlayerController::OnDown() {

    }

    void SidePlayerController::SetPlayerCharacter(Actors::PlayerCharacter *Character) {
        m_PlayerCharacter = Character;
        SetStateMachineOnPlayer<Actors::CharacterMovementStateMachineGrounded>();
    }

    void SidePlayerController::OnShoot() {
        if(!m_startShooting) {
            m_startShooting = true;
        } else
        {
            if(m_shootCallback)
                m_shootCallback();
            m_startShooting = false;
        }
    }
}