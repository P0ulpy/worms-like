#include "PhysicsEvents.hpp"
#include "Engine/Core/Components/Physics.hpp"

void Game::PhysicsEvents::WindApplier::OnUpdate(const float& dt) {
    GetScene()->GetRegistry()->View<Engine::Components::Physics::RigidBody2DdComponent>([](Engine::Component* Component)->void {
        auto Casted = static_cast<Engine::Components::Physics::RigidBody2DdComponent*>(Component);
        if (!Casted->IsStatic) {
            Casted->GetRigidBody()->AddForce(Maths::Vector2D<double>(5.f, 1.f), Maths::Vector2D<double>(0.f, 0.f));
        }
    });
}
