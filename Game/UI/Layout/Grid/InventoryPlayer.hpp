//
// Created by Admin on 28/02/2023.
//

#pragma once


#include "Engine/UI/Layout/Grid/WidgetGrid.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace Game::UI
{
    class InventoryPlayer : public Engine::UI::WidgetGrid {
    public:
        InventoryPlayer() = default;

        void Init(const sf::Vector2u& gridSize, const sf::Vector2f& gridSpacing, const sf::Vector2f& position, const sf::Texture* background, const sf::Texture* itemBackground);

        void OnRender(sf::RenderTarget& renderTarget) override;

        void SetPosition(const sf::Vector2f& position) override;

        void AddChild(Engine::UI::Widget* child) override;

    private:
        sf::Vector2u m_sizeBackground;
        sf::Sprite m_background;
        const sf::Texture* m_itemBackground { nullptr };
    };
}
