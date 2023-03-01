//
// Created by Flo on 22/02/2023.
//

#include "WidgetHorizontalBox.hpp"

namespace Engine::UI {

    void WidgetHorizontalBox::Init(const sf::Vector2f &position, const float spacing) {
        m_spacing = spacing;
        SetPosition(position);
    }

    void WidgetHorizontalBox::UpdatePosition() const {
        auto position = GetPosition();
        float x = position.x;
        auto children = GetChildren();

        for (auto& child : children) {
            child->SetPosition({x, position.y});
            x += child->GetSize().x + m_spacing;
        }

        UpdateSize();
    }

    void WidgetHorizontalBox::UpdateSize() const {
        auto children = GetChildren();
        float width = 0.0f;
        float height = 0.0f;

        for (auto& child : children) {
            width += child->GetSize().x + m_spacing;
            height = std::max(height, child->GetSize().y);
        }

        width -= m_spacing;

        m_size = {width, height};
    }


} // UI