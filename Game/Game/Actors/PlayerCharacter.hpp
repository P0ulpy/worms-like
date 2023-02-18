#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "Engine/Core/Components/Physics.hpp"
#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"

namespace Game::Actors {
    struct CharacterMovementStateMachine {
        using OnEvent = std::function<void()>;

        // @todo with Signals maybe?
        OnEvent OnJump;
        OnEvent OnLeft;
        OnEvent OnRight;
        OnEvent OnDown;

        virtual void HandleJump() const = 0;
        virtual void HandleLeft() const = 0;
        virtual void HandleRight() const = 0;
        virtual void HandleDown() const = 0;

        virtual ~CharacterMovementStateMachine() = default;
    };

    struct CharacterMovementStateMachineJumping : public CharacterMovementStateMachine {
        void HandleJump() const override {/*ignore*/}
        void HandleLeft() const override {/*ignore*/}
        void HandleRight() const override {/*ignore*/}
        void HandleDown() const override {/*ignore*/}
    };

    struct CharacterMovementStateMachineGrounded : public CharacterMovementStateMachine {
        void HandleJump() const override {
            OnJump();
        }

        void HandleLeft() const override {
            OnLeft();
        }
        void HandleRight() const override {
            OnRight();
        }

        void HandleDown() const override {
            OnDown();
        }
    };

    class PlayerCharacter : public Engine::Component {
    public:
        using RequiredComponents = Engine::ComponentRequirements<Engine::Components::Physics::RigidBody2DdComponent>;

        DECLARE_CLASS_TYPE(PlayerCharacter, Engine::Component)

        void OnAwake();

        ~PlayerCharacter() override;
    private:
        SignalSystem::ScopedConnectionSignal m_KeyLeftConnection;
        SignalSystem::ScopedConnectionSignal m_KeyRightConnection;
        SignalSystem::ScopedConnectionSignal m_KeyJumpConnection;
        SignalSystem::ScopedConnectionSignal m_KeyDownConnection;

        template <typename ST>
        void SetStateMachine()
        {
            static_assert(std::is_base_of_v<CharacterMovementStateMachine, ST>);

            delete StateMachine;
            StateMachine = new ST();
            StateMachine->OnJump = [this]()->void {OnJump();};
            StateMachine->OnLeft = [this]()->void {OnLeft();};
            StateMachine->OnRight = [this]()->void {OnRight();};
            StateMachine->OnDown = [this]()->void {OnDown();};
        }

        void CallJump();
        void CallLeft();
        void CallRight();
        void CallDown();

        void OnJump();
        void OnLeft();
        void OnRight();
        void OnDown();

        CharacterMovementStateMachine* StateMachine = nullptr;
    };
}