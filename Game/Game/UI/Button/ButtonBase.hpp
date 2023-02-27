//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_BUTTONBASE_HPP
#define POPOSIBENGINE_BUTTONBASE_HPP


#include <functional>
#include "../IWidget.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Mouse.hpp"

#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"

class ButtonBase : public IWidget {
public:
    using Callback = std::function<void()>;

    public:
    explicit ButtonBase()
        : m_defaultColor(sf::Color::White), m_hoverColor(sf::Color::Red), m_clickedColor(sf::Color::Green)
    {
        m_button.setFillColor(sf::Color::Transparent);
    }

    void OnUpdateWidget(const float& deltaTime) override
    {
        if(m_button.getGlobalBounds().contains(sf::Vector2f{sf::Mouse::getPosition(Engine::EngineApplication::Get()->GetWindow())}))
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

    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        renderTarget.draw(m_button);
    }

    void AddWidget(IWidget* child) override {  }
    void RemoveWidget(IWidget* child) override {  }

    void Init(const sf::Vector2f& position, const sf::Vector2f& size)
    {
        SetPosition(position);
        SetSize(size);
    }

    void SetPosition(const sf::Vector2f& position) override {
        IWidget::SetPosition(position);
        m_button.setPosition(position);
    }
    void SetSize(const sf::Vector2f& size) override {
        IWidget::SetSize(size);
        m_button.setSize(size);
    }

    void SetDefaultColor(const sf::Color& color) { m_defaultColor = color; }
    void SetHoverColor(const sf::Color& color) { m_hoverColor = color; }
    void SetClickedColor(const sf::Color& color) { m_clickedColor = color; }

    void SetOnClick(const Callback& onClick) {
        m_onClick = onClick;
        m_OnClickConnection = InputSignal::GetInstance()->connectScoped(EventType::Click, this, &ButtonBase::OnClick);
    }

    virtual void OnClick()
    {
        if(m_isHovered && isVisibled)
            m_onClick();
    }

protected:
    sf::RectangleShape m_button;

    sf::Color m_defaultColor = sf::Color::White;
    sf::Color m_hoverColor = sf::Color::Red;
    sf::Color m_clickedColor = sf::Color::Green;

    bool m_isHovered = false;

    std::function<void()> m_onClick;

    ScopedConnection m_OnClickConnection;
};


#endif //POPOSIBENGINE_BUTTONBASE_HPP
