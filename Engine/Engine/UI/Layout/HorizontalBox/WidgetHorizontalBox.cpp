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
    }


} // UI