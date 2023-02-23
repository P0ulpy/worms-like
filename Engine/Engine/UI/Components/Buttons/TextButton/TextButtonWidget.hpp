//
// Created by Flo on 22/02/2023.
//

#ifndef WORMS_LIKE_TEXTBUTTONWIDGET_HPP
#define WORMS_LIKE_TEXTBUTTONWIDGET_HPP

#include "../ButtonWidget.hpp"
#include "SFML/Graphics/Text.hpp"

namespace Engine::UI
{
    class TextButtonWidget : public ButtonWidget
    {
    public:

        void OnUpdate(const float& deltaTime) override
        {
            auto mousePos = sf::Vector2f{ sf::Mouse::getPosition(Engine::EngineApplication::Get()->GetWindow()) };

            if(m_button.getGlobalBounds().contains(mousePos))
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
        }

        void OnRender(sf::RenderTarget& renderTarget) override
        {
            ButtonWidget::OnRender(renderTarget);
            renderTarget.draw(m_text);
        }

        void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, const sf::Vector2f& size, const float& border = 0.0f, unsigned int characterSize = 30)
        {
            m_border = border;
            m_text.setFont(*font);
            m_text.setCharacterSize(characterSize);
            m_text.setFillColor(m_defaultColor);
            SetText(text);

            sf::Vector2f textButtonSize = {
                m_text.getLocalBounds().width + m_border * 2,
                m_text.getLocalBounds().height + m_border * 2
            };

            SetSize(textButtonSize);
            SetPosition(position);
        }

        void SetText(const std::string& text) { m_text.setString(text); }

        void SetPosition(const sf::Vector2f& position) override
        {
            ButtonWidget::SetPosition(position);
            float xPos = (position.x + GetSize().x / 2) - (m_text.getLocalBounds().width / 2);
            float yPos = (position.y + GetSize().y / 2.2f) - (m_text.getLocalBounds().height / 2);
            m_text.setPosition(xPos, yPos);
        }

        void SetSize(const sf::Vector2f& size) override { ButtonWidget::SetSize(size); }

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
} // UI

#endif //WORMS_LIKE_TEXTBUTTONWIDGET_HPP
