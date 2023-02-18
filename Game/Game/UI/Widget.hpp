//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_WIDGET_HPP
#define POPOSIBENGINE_WIDGET_HPP


#include "Engine/ECS/Component/Component.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class Widget : public Engine::Component {
public:
    DECLARE_CLASS_TYPE(Widget, Engine::Component)

public:
    Widget() = default;
    ~Widget() override = default;

    virtual void OnAwake() {  };
    virtual void OnStart() {  };
    virtual void OnUpdate(const float& deltaTime) { };
    virtual void OnRender(sf::RenderTarget& renderTarget) { };
    virtual void OnImGuiRender() {  };
    virtual void OnDestroy() {  };
};


#endif //POPOSIBENGINE_WIDGET_HPP
