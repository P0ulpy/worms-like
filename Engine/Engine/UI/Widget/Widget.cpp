//
// Created by Flo on 21/02/2023.
//

#include "Widget.hpp"

namespace Engine::UI
{
    void Widget::AddChild(Widget *child)
    {
        CompositeComponent<Widget>::AddChild(child);
        UpdatePosition();
    }

    void Widget::RemoveChild(Widget *child) const
    {
        CompositeComponent<Widget>::RemoveChild(child);
        UpdatePosition();
    }

    void Widget::SetPosition(const sf::Vector2f& position)
    {
        m_position = position;
        UpdatePosition();
    }

    void Widget::SetSize(const sf::Vector2f &size)
    {
        m_size = size;
        UpdateSize();
    }
} // Engine::UI