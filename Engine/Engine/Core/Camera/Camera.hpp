#pragma once

#include <cstddef>
#include <SFML/Graphics.hpp>
#include "../../ECS/ComponentSystem/IComponentSystem.hpp"
#include "../../Maths/Vector.hpp"

namespace Engine::Camera {
    class ICamera {
    public:
        virtual void Render(
            sf::RenderTarget& RenderTarget,
            const std::unordered_map<RTTI::ClassType*, IComponentSystem*>& RenderableSystems
        ) = 0;

        virtual ~ICamera() = default;
    };

    enum struct CameraSizeType
    {
        FILL = 0,
        PROPORTIONAL,
        FIXED
    };

    template <typename GeometricT>
    class Camera2D : public ICamera {
    public:
        Camera2D() = default;

        // numbers of pixels for 1 centimeter
        GeometricT PixelCentimeterRatio = 0.5f;
        Maths::Point<GeometricT, 2> Position;
        CameraSizeType SizeType = CameraSizeType::FILL;
        // if SizeType FIXED, will be used as is, if PROPORTIONAL will be multiplied with RenderTarget size
        Maths::Vector<GeometricT, 2> Size;

        void Render(
            sf::RenderTarget& RenderTarget,
            const std::unordered_map<RTTI::ClassType*, IComponentSystem*>& RenderableSystems
        ) override
        {
            const auto RenderTargetSize = RenderTarget.getSize();
            if (CameraSizeType::FIXED == SizeType) {
                m_CurrentSize = Size;
            } else {
                auto SizeX = RenderTargetSize.x / PixelCentimeterRatio;
                auto SizeY = RenderTargetSize.x / PixelCentimeterRatio;
                switch (SizeType) {
                    case CameraSizeType::PROPORTIONAL:
                        m_CurrentSize = Maths::Vector<GeometricT, 2>((double) SizeX * Size.GetX(), (double) SizeY * Size.GetY());
                    case CameraSizeType::FILL:
                    default:
                        m_CurrentSize = Maths::Vector<GeometricT, 2>((double) SizeX, (double) SizeY);
                        break;
                }
            }
            sf::View View;
            View.setSize((float) m_CurrentSize.GetX(), (float) m_CurrentSize.GetY());
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

        const Maths::Vector<GeometricT, 2>& GetCurrentSize()
        {
            return m_CurrentSize;
        }
    private:
        Maths::Vector<GeometricT, 2> m_CurrentSize;
    };
}
