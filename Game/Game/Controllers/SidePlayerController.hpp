#pragma once

#include <utility>

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Core/ScenesSystem/ScenesLayer/ScenesLayer.hpp"
#include "Engine/Core/Inputs/WindowEvents.hpp"
#include "../Actors/PlayerCharacter.hpp"

namespace Game::Controllers {
    class SidePlayerController : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(SidePlayerController, Component)

        void OnAwake();
        void OnUpdate(const float& DeltaTime);

        Maths::Point2D<double> MinPosition = Maths::Point2D<double>(0.f, 0.f);
        Maths::Point2D<double> MaxPosition = Maths::Point2D<double>(0.f, 0.f);

        void SetPlayerCharacter(Actors::PlayerCharacter* Character);
        void SetWeaponToShoot(std::function<void()> shootCallback) { m_shootCallback = std::move(shootCallback); }
    private:
        // ratio of size/s
        static float constexpr CameraXRatioSpeed = 0.1f;
        // ratio of size/s
        static float constexpr CameraYRatioSpeed = 0.1f;
        static float constexpr MinRatioToMoveCamera = 0.85f;
        static float constexpr MaxRatioToMoveCamera = 1.f;

        SignalSystem::ScopedConnectionSignal m_KeyLeftConnection;
        SignalSystem::ScopedConnectionSignal m_KeyRightConnection;
        SignalSystem::ScopedConnectionSignal m_KeyJumpConnection;
        SignalSystem::ScopedConnectionSignal m_KeyDownConnection;
        SignalSystem::ScopedConnectionSignal m_shootConnection;

        std::function<void()> m_shootCallback;
        bool m_startShooting = false;

        void OnJump();
        void OnLeft();
        void OnRight();
        void OnDown();
        void OnShoot();

        void HandleCameraMovementFromMousePos(const float& DeltaTime);

        Game::Actors::PlayerCharacter* m_PlayerCharacter = nullptr;

        template <typename ST>
        void SetStateMachineOnPlayer()
        {
            if (nullptr == m_PlayerCharacter) return;

            static_assert(std::is_base_of_v<Actors::CharacterMovementStateMachine, ST>);

            delete m_PlayerCharacter->StateMachine;
            m_PlayerCharacter->StateMachine = new ST();
            m_PlayerCharacter->StateMachine->OnJump = [this]()->void {OnJump();};
            m_PlayerCharacter->StateMachine->OnLeft = [this]()->void {OnLeft();};
            m_PlayerCharacter->StateMachine->OnRight = [this]()->void {OnRight();};
            m_PlayerCharacter->StateMachine->OnDown = [this]()->void {OnDown();};
            m_PlayerCharacter->StateMachine->OnShoot = [this]()->void {OnShoot();};
        }
    };
}
