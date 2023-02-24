//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Text.hpp"

namespace Engine::UI {

        class TextWidget : public Widget {
        public:
            virtual void OnUpdate(float deltaTime);
            virtual void OnRender(sf::RenderTarget& renderTarget);

            void Init(const std::string& text, sf::Font* font, const sf::Vector2f& position, unsigned int characterSize = 30,  const sf::Color& color = sf::Color::White);

            void SetPosition(const sf::Vector2f& position) override;

        private:
            sf::Text m_text;
        };

} // UI
