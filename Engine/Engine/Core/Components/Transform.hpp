//
// Created by Flo on 18/01/2023.
//
#pragma once

#include "../../ECS/Component/Component.hpp"
#include <SFML/Graphics/Transform.hpp>
#include "../../Maths/Vector.hpp"

namespace Engine::Components {
    class Transform : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(Transform, Engine::Component)

        using PointT = Maths::Point2D<double>;
        using VectorT = Maths::Vector2D<double>;

        PointT Pos = PointT(.0f, .0f);
        VectorT Scale = VectorT(1.f, 1.f);
        float Angle = .0f;

        sf::Transform TransformationMatrix;
    };

} // Engine
