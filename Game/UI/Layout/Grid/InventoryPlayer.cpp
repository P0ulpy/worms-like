//
// Created by Admin on 28/02/2023.
//

#include "InventoryPlayer.hpp"

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace Game::UI
{
    void InventoryPlayer::Init(const sf::Vector2u &gridSize, const sf::Vector2f &gridSpacing, const sf::Vector2f &position,
                               const sf::Texture *background, const sf::Texture *itemBackground)
    {
        WidgetGrid::Init(gridSize, gridSpacing, position, { 0.0f, 0.0f });
        m_background.setTexture(*background);
        m_background.setPosition(position);

        m_sizeBackground = m_background.getTexture()->getSize();
        m_itemBackground = itemBackground;
    }

    void InventoryPlayer::OnRender(sf::RenderTarget &renderTarget) {
        renderTarget.draw(m_background);

        for (auto& child : GetChildren())
        {
            if(nullptr != m_itemBackground)
            {
                sf::Sprite sprite;
                sprite.setTexture(*m_itemBackground);
                sprite.setPosition(child->GetPosition());
                sprite.setScale(child->GetSize().x / m_itemBackground->getSize().x, child->GetSize().y / m_itemBackground->getSize().y);
                renderTarget.draw(sprite);
            }

            child->OnRender(renderTarget);
        }
    }

    void InventoryPlayer::SetPosition(const sf::Vector2f &position) {
        Widget::SetPosition(position);
        m_background.setPosition(position);
    }

    void InventoryPlayer::AddChild(Engine::UI::Widget *child) {
        WidgetGrid::AddChild(child);
        m_background.setScale(GetSize().x / static_cast<float>(m_sizeBackground.x), GetSize().y / static_cast<float>(m_sizeBackground.y));
    }
}
