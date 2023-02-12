//
// Created by Flo on 18/01/2023.
//

#include "../../ECS/Component/Component.hpp"
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

#ifndef POPOSIBENGINE_TRANSFORM_HPP
#define POPOSIBENGINE_TRANSFORM_HPP

namespace Engine {

    class Transform : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(Transform, Engine::Component)

        sf::Vector2<float> Pos { .0f, .0f };
        sf::Vector2<float> Scale { 1.f, 1.f };
        float Angle = .0f;

        sf::Transform TransformationMatrix;
    };

} // Engine

#endif //POPOSIBENGINE_TRANSFORM_HPP