#include "SidePlayerController.hpp"

namespace Game::Controllers {
    void SidePlayerController::OnUpdate(const float& DeltaTime)
    {
        HandleCameraMovementFromMousePos(DeltaTime);
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
}