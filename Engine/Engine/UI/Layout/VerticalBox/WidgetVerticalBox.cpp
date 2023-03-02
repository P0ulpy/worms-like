//
// Created by Flo on 22/02/2023.
//

#include "WidgetVerticalBox.hpp"

namespace Engine::UI {

    void WidgetVerticalBox::Init(const sf::Vector2f &position, const float spacing) {
        m_spacing = spacing;
        SetPosition(position);
    }

    void WidgetVerticalBox::UpdatePosition() const {
        auto position = GetPosition();
        float y = position.y;
        auto children = GetChildren();

        for (auto& child : children) {
            child->SetPosition({position.x, y});
            y += child->GetSize().y + m_spacing;
        }

        UpdateSize();
    }

    void WidgetVerticalBox::UpdateSize() const {
        auto children = GetChildren();
        float width = 0.0f;
        float height = 0.0f;

        for (auto& child : children) {
            width = std::max(width, child->GetSize().x);
            height += child->GetSize().y + m_spacing;
        }

        height -= m_spacing;

        m_size = {width, height};
    }
} // UI