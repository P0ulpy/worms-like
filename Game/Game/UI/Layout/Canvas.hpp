//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_CANVAS_HPP
#define POPOSIBENGINE_CANVAS_HPP

#include "Engine/ECS/Component/Component.hpp"
#include "../WidgetComposite.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"

class Canvas : public WidgetComposite , public Engine::Component {
public:
    DECLARE_CLASS_TYPE(Canvas, Engine::Component)

public:
    Canvas() = default;
    ~Canvas() override = default;

    void AddWidget(IWidget *child) override {
        m_children.push_back(child);
        child->SetPosition(m_position + child->GetPosition());
    }

    void OnAwake() {}
    void OnStart() {}
    void OnUpdate(const float& deltaTime) {
        if(isVisibled)
            OnUpdateWidget(deltaTime);
    }
    void OnRender(sf::RenderTarget& renderTarget) {
        if(isVisibled)
            OnRenderWidget(renderTarget);
    }
    void OnImGuiRender() {}
    void OnDestroy() {}

    void Init(const sf::Vector2f& position = {0, 0}, const sf::Vector2f& size = {static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().x), static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().y)}) {
        m_position = position;
        m_size = size;
    }
};


#endif //POPOSIBENGINE_CANVAS_HPP
