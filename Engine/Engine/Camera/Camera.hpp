#pragma once

#include <cstddef>
#include <SFML/Graphics.hpp>
#include "../ECS/ComponentSystem/IComponentSystem.hpp"
#include "../Maths/Vector.hpp"

namespace Engine::Camera {
    class ICamera {
    public:
        virtual void Render(
            sf::RenderTarget& RenderTarget,
            const std::unordered_map<RTTI::ClassType*, IComponentSystem*>& RenderableSystems
        ) = 0;

        virtual ~ICamera() = default;
    };

    template <typename GeometricT>
    class Camera2D : public ICamera {
    public:
        Camera2D() = default;

        // numbers of pixels for 1 centimeter
        GeometricT PixelCentimeterRatio = 1;
        Maths::Point<GeometricT, 2> Position;

        void Render(
            sf::RenderTarget& RenderTarget,
            const std::unordered_map<RTTI::ClassType*, IComponentSystem*>& RenderableSystems
        ) override
        {
            const auto RenderTargetSize = RenderTarget.getSize();
            sf::View View;
            View.setSize((float) RenderTargetSize.x / PixelCentimeterRatio, (float) RenderTargetSize.y / PixelCentimeterRatio);
            View.setCenter({
               (float) Position.GetX(),
               (float) Position.GetY()
            });
            RenderTarget.setView(View);
            for (auto& [Class, RenderableSystem] : RenderableSystems)
            {
                RenderableSystem->DispatchRender(
                    RenderTarget
                );
            }
        }
    };
}
