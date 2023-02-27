//
// Created by Admin on 20/02/2023.
//

#pragma once

#include "ButtonBase.hpp"
#include "SFML/Graphics/Sprite.hpp"

class SpriteButton : public ButtonBase {
public:
    explicit SpriteButton() = default;
    ~SpriteButton() override = default;

    void OnUpdateWidget(const float& deltaTime) override {
        if(m_button.getGlobalBounds().contains(sf::Vector2f{sf::Mouse::getPosition(Engine::EngineApplication::Get()->GetWindow())}))
        {
            if(!m_isHovered)
                m_isHovered = true;
        }
        else
        {
            if(m_isHovered)
                m_isHovered = false;
        }
    };
    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        ButtonBase::OnRenderWidget(renderTarget);
        renderTarget.draw(m_sprite);
    };

    void AddWidget(IWidget* child) override {  };
    void RemoveWidget(IWidget* child) override {  };

    void Init(const sf::Texture* texture, const sf::Vector2f& position, const float& border = 0.0f)
    {
        m_border = border;
        m_sprite.setTexture(*texture);
        SetSize({ m_sprite.getGlobalBounds().width + m_border * 2, m_sprite.getGlobalBounds().height + m_border * 2 });
        SetPosition(position);
    }

    void SetPosition(const sf::Vector2f& position) override {
        ButtonBase::SetPosition(position);
        m_sprite.setPosition(m_position + sf::Vector2f{m_border, m_border});
    }
    void SetSize(const sf::Vector2f& size) override {
        ButtonBase::SetSize(size);
    }

    const sf::Sprite& GetSprite() const { return m_sprite; }

private:
    sf::Sprite m_sprite;
    float m_border = 0.0f;
};
