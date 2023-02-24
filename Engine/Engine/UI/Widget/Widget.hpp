//
// Created by Flo on 21/02/2023.
//

#pragma once

#include "../../Core/Components/CompositeComponent.hpp"
#include "SFML/System/Vector2.hpp"

namespace Engine::UI
{
    class Widget : public CompositeComponent<Widget>
    {
    public:
        DECLARE_CLASS_TYPE(Widget, CompositeComponent<Widget>)

        void AddChild(Widget* child) override;
        void RemoveChild(Widget* child) const override;

        virtual void SetPosition(const sf::Vector2f& position);
        virtual void SetSize(const sf::Vector2f& size);

        [[nodiscard]] const sf::Vector2f& GetPosition() const   { return m_position; }
        [[nodiscard]] const sf::Vector2f& GetSize() const       { return m_size; }

        virtual void UpdatePosition() const   {}
        virtual void UpdateSize() const       {}

    protected:
        mutable sf::Vector2f m_position { .0f, .0f };
        mutable sf::Vector2f m_size { .0f, .0f };
    };
} // Engine::UI
