//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_BUTTON_HPP
#define POPOSIBENGINE_BUTTON_HPP


#include <functional>
#include <utility>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Inputs/InputSignal.h"
#include "../Widget.hpp"

class Button : public Widget {
public:
    DECLARE_CLASS_TYPE(Button, Widget)

    using Callback = std::function<void()>;

public:
    explicit Button()
        : m_defaultColor(sf::Color::White), m_hoverColor(sf::Color::Red), m_clickedColor(sf::Color::Green)
    {}

    void OnAwake() {  };
    void OnStart() {  };
    void OnUpdate(const float& deltaTime) {
        if(m_button.getGlobalBounds().contains(sf::Vector2f{sf::Mouse::getPosition(Engine::EngineApplication::Get()->GetWindow())}))
        {
            if(!m_isHovered)
            {
                m_isHovered = true;
                m_text.setFillColor(m_hoverColor);
            }
        }
        else
        {
            if(m_isHovered)
            {
                m_isHovered = false;
                m_text.setFillColor(m_defaultColor);
            }
        }
    };
    void OnRender(sf::RenderTarget& renderTarget) {
        renderTarget.draw(m_button);
        renderTarget.draw(m_text);
    };
    void OnImGuiRender() {  };
    void OnDestroy() {  };

    void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, const sf::Vector2f& size, unsigned int characterSize = 30)
    {
        m_text.setFont(*font);
        m_text.setCharacterSize(characterSize);
        m_text.setFillColor(m_defaultColor);
        m_button.setFillColor(sf::Color::Transparent);
        SetText(text);
        SetPosition(position);
        SetSize(size);

        sf::FloatRect textRect = m_text.getGlobalBounds();
        m_text.setPosition(position.x + size.x / 2 - textRect.width / 2, position.y + size.y / 2 - textRect.height / 2);
    }

    void SetOnClick(const Callback& onClick) {
        m_onClick = onClick;
        m_OnClickConnection = InputSignal::GetInstance()->connectScoped(EventType::Click, this, &Button::OnClick);
    }

    void SetPosition(const sf::Vector2f& position) { m_button.setPosition(position); }
    void SetSize(const sf::Vector2f& size) { m_button.setSize(size); }
    void SetText(const std::string& text) { m_text.setString(text); }
    void SetDefaultColor(const sf::Color& color) { m_defaultColor = color; }
    void SetHoverColor(const sf::Color& color) { m_hoverColor = color; }
    void SetClickedColor(const sf::Color& color) { m_clickedColor = color; }

    void OnClick()
    {
        if(m_isHovered)
        {
            m_text.setFillColor(sf::Color::Green);
            m_onClick();
        }
    }

private:
    sf::RectangleShape m_button;
    sf::Text m_text;

    sf::Color m_defaultColor = sf::Color::White;
    sf::Color m_hoverColor = sf::Color::Red;
    sf::Color m_clickedColor = sf::Color::Green;

    bool m_isHovered = false;

    std::function<void()> m_onClick;

    ScopedConnection m_OnClickConnection;
};


#endif //POPOSIBENGINE_BUTTON_HPP
