//
// Created by Flo on 22/02/2023.
//

#include "ButtonWidget.hpp"

namespace Engine::UI
{
    ButtonWidget::ButtonWidget()
            : m_defaultColor(sf::Color::White)
            , m_hoverColor(sf::Color::Red)
            , m_clickedColor(sf::Color::Green)
    {
        m_button.setFillColor(sf::Color::Transparent);
    }

    void ButtonWidget::OnUpdate(const float &deltaTime)
    {
        auto mousePos = sf::Vector2f{ sf::Mouse::getPosition(Engine::EngineApplication::Get()->GetWindow()) };

        if(m_button.getGlobalBounds().contains(mousePos))
        {
            if(!m_isHovered)
            {
                m_isHovered = true;
                m_button.setFillColor(m_hoverColor);
            }
        }
        else
        {
            if(m_isHovered)
            {
                m_isHovered = false;
                m_button.setFillColor(m_defaultColor);
            }
        }
    }

    void ButtonWidget::OnRender(sf::RenderTarget &renderTarget) {
        renderTarget.draw(m_button);
    }

    void ButtonWidget::Init(const sf::Vector2f &position, const sf::Vector2f &size)
    {
        SetPosition(position);
        SetSize(size);
    }

    void ButtonWidget::SetPosition(const sf::Vector2f &position) {
        Widget::SetPosition(position);
        m_button.setPosition(position);
    }

    void ButtonWidget::SetSize(const sf::Vector2f &size) {
        Widget::SetSize(size);
        m_button.setSize(size);
    }

    void ButtonWidget::SetOnClick(const ButtonWidget::Callback &onClick) {
        m_onClick = onClick;
        m_OnClickConnection.~ScopedConnection();
        m_OnClickConnection = InputSignal::GetInstance()->connectScoped(EventType::Click, this, &ButtonWidget::OnClick);
    }

    void ButtonWidget::OnClick() {
        if(m_isHovered && IsActive())
            m_onClick();
    }
}

