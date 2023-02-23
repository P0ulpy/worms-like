//
// Created by Flo on 22/02/2023.
//

#ifndef WORMS_LIKE_WIDGETCANVAS_HPP
#define WORMS_LIKE_WIDGETCANVAS_HPP


#include "../Widget/Widget.hpp"
#include "../../Core/Application/EngineApplication.hpp"

namespace Engine::UI
{
        class WidgetCanvas : public Widget
        {
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

#endif //WORMS_LIKE_WIDGETCANVAS_HPP
