#pragma once


#include "Engine/ECS/Component/Component.hpp"

namespace Game::PhysicsEvents {
    class WindApplier : public Engine::Component {
        void OnUpdate(const float& dt);
    };
}
