//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace Engine::UI {

        class SpriteWidget : public Widget {
        public:
            virtual void OnRender(sf::RenderTarget& renderTarget);

            void Init(const sf::Texture& texture, const sf::Vector2f& position);

            void SetPosition(const sf::Vector2f& position) override;
            void SetScale(const sf::Vector2f& scale);

        private:
            void UpdateSize() const override;

        private:
            sf::Sprite m_sprite;
        };
} // UI
