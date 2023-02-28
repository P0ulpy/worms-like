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

            void OnRender(sf::RenderTarget& target) override
            {
                auto children = GetChildren();

                std::sort(children.begin(), children.end(), [](const auto& lhs, const auto& rhs) {
                    return lhs->GetLayoutIndex() < rhs->GetLayoutIndex();
                });

                for (auto& child : children)
                {
                    child->OnRender(target);
                }
            }

            void AddChild(Widget *child, int layout = 0)
            {
                Widget::AddChild(child);
                child->SetPosition(GetPosition() + child->GetPosition());
                child->SetLayoutIndex(layout);
            }

            void SetPosition(const sf::Vector2f& position) override
            {
                m_previousPosition = position;
                Widget::SetPosition(position);
            }

        private:
            sf::Vector2f m_previousPosition {0, 0};

            void UpdatePosition() const override
            {
                auto delta = GetPosition() - m_previousPosition;

                auto children = GetChildren();
                for (auto& child : children)
                {
                    child->SetPosition(child->GetPosition() + delta);
                }
            }
        };
} // UI
