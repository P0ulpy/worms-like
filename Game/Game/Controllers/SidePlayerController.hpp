#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "Engine/Core/ScenesSystem/ScenesLayer/ScenesLayer.hpp"
#include "Engine/Core/Inputs/WindowEvents.hpp"
#include "../Actors/PlayerCharacter.hpp"

namespace Game::Controllers {
    class SidePlayerController : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(SidePlayerController, Component)

        void OnUpdate(const float& DeltaTime);

        Maths::Point2D<double> MinPosition = Maths::Point2D<double>(0.f, 0.f);
        Maths::Point2D<double> MaxPosition = Maths::Point2D<double>(0.f, 0.f);

        Game::Actors::PlayerCharacter* PlayerCharacter = nullptr;
    private:
        // ratio of size/s
        static float constexpr CameraXRatioSpeed = 0.1f;
        // ratio of size/s
        static float constexpr CameraYRatioSpeed = 0.1f;
        static float constexpr MinRatioToMoveCamera = 0.85f;
        static float constexpr MaxRatioToMoveCamera = 1.f;

        void HandleCameraMovementFromMousePos(const float& DeltaTime);
    };
}
