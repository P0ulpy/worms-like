//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../Widget/Widget.hpp"
#include "../../Core/Application/EngineApplication.hpp"

namespace Engine::UI
{
        class WidgetCanvas : public Widget
        {
        public:
            DECLARE_CLASS_TYPE(WidgetCanvas, Widget)

            void Init(const sf::Vector2f& position = {0, 0}, const sf::Vector2f& size = {
                    static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().x),
                    static_cast<float>(Engine::EngineApplication::Get()->GetWindow().getSize().y)
                }
            )
            {
                SetPosition(position);
                SetSize(size);
            }

            void AddChild(Widget *child) override
            {
                Widget::AddChild(child);
                child->SetPosition(GetPosition() + child->GetPosition());
            }
        };
} // UI
