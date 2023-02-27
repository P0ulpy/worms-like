//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_TEXTBUTTON_HPP
#define POPOSIBENGINE_TEXTBUTTON_HPP

#include <functional>
#include <utility>

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/Core/Inputs/Connection.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"
#include "ButtonBase.hpp"

class TextButton : public ButtonBase {
public:
    explicit TextButton() = default;
    ~TextButton() override = default;

    void OnUpdateWidget(const float& deltaTime) override {
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
    void OnRenderWidget(sf::RenderTarget& renderTarget) {
        ButtonBase::OnRenderWidget(renderTarget);
        renderTarget.draw(m_text);
    };

    void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, const sf::Vector2f& size, const float& border = 0.0f, unsigned int characterSize = 30)
    {
        m_border = border;
        m_text.setFont(*font);
        m_text.setCharacterSize(characterSize);
        m_text.setFillColor(m_defaultColor);
        SetText(text);

        //SetSize(size);
        SetSize({m_text.getLocalBounds().width + m_border * 2, m_text.getLocalBounds().height + m_border * 2});
        SetPosition(position);
    }

    void SetText(const std::string& text) { m_text.setString(text); }

    void SetPosition(const sf::Vector2f& position) override
    {
        ButtonBase::SetPosition(position);
        float xPos = (position.x + m_size.x / 2) - (m_text.getLocalBounds().width / 2);
        float yPos = (position.y + m_size.y / 2.2f) - (m_text.getLocalBounds().height / 2);
        m_text.setPosition(xPos, yPos);
    }

    void SetSize(const sf::Vector2f& size) override { ButtonBase::SetSize(size); }

    void OnClick() override
    {
        if(m_isHovered)
        {
            m_text.setFillColor(sf::Color::Green);
            m_onClick();
        }
    }

private:
    sf::Text m_text;
    float m_border = 0;
};


#endif //POPOSIBENGINE_TEXTBUTTON_HPP
