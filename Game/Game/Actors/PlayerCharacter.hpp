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
        OnEvent OnShoot;

        bool isLock = false;

        void LockMouvement() { isLock = true; }
        void UnlockMouvement() { isLock = false; }

        virtual void HandleJump() const = 0;
        virtual void HandleLeft() const = 0;
        virtual void HandleRight() const = 0;
        virtual void HandleDown() const = 0;
        virtual void HandleShoot() const = 0;

        virtual ~CharacterMovementStateMachine() = default;
    };

    struct CharacterMovementStateMachineJumping : public CharacterMovementStateMachine {
        void HandleJump() const override {/*ignore*/}
        void HandleLeft() const override {if(isLock) return; OnLeft();}
        void HandleRight() const override {if(isLock) return; OnRight();}
        void HandleDown() const override {if(isLock) return; OnDown();}
        void HandleShoot() const override {if(isLock) return; OnDown();}
    };

    struct CharacterMovementStateMachineGrounded : public CharacterMovementStateMachine {
        void HandleJump() const override {
            if(isLock) return;
            OnJump();
        }

        void HandleLeft() const override {
            if(isLock) return;
            OnLeft();
        }
        void HandleRight() const override {
            if(isLock) return;
            OnRight();
        }

        void HandleDown() const override {
            /*ignore*/
        }

        void HandleShoot() const override {
            OnShoot();
        }
    };

    class PlayerCharacter : public Engine::Component {
    public:
        using RequiredComponents = Engine::ComponentRequirements<Engine::Components::Physics::RigidBody2DdComponent>;
        DECLARE_CLASS_TYPE(PlayerCharacter, Engine::Component)

        void OnAwake();
        void Init(sf::Texture* texture);

        void OnUpdate(const float& DeltaTime);
        void OnRender(sf::RenderTarget& renderTarget);

        ~PlayerCharacter() override;

        CharacterMovementStateMachine* StateMachine = nullptr;
    private:
        template <typename ST>
        void SetStateMachine()
        {
            static_assert(std::is_base_of_v<CharacterMovementStateMachine, ST>);

            delete StateMachine;
            StateMachine = new ST();
            StateMachine->OnJump = [this]()->void {/*ignore*/};
            StateMachine->OnLeft = [this]()->void {/*ignore*/};
            StateMachine->OnRight = [this]()->void {/*ignore*/};
            StateMachine->OnDown = [this]()->void {/*ignore*/};
            StateMachine->OnShoot = [this]()->void {/*ignore*/};
        }

        sf::Sprite m_sprite;
    };
}
