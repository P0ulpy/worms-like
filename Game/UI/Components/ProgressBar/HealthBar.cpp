//
// Created by Admin on 28/02/2023.
//

#include "HealthBar.hpp"

namespace Game::UI {
    void HealthBar::OnUpdate(Engine::Timestep ts) {
        m_Value = 100.0f;
        m_FillSprite.setScale(GetSize().x * (m_Value / m_MaxValue) / m_FillGlobalBounds.x,
                              GetSize().y / m_FillGlobalBounds.y);
    }

    void HealthBar::Init(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Texture &backgroundTexture,
                         const sf::Texture &fillTexture) {
        ProgressBarWidget::Init(position, size, backgroundTexture, fillTexture, {0.0f, 0.0f});
    }
}