//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_SPRITE_HPP
#define POPOSIBENGINE_SPRITE_HPP


#include "../IWidget.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Sprite : public IWidget {
public:
    Sprite() = default;
    ~Sprite() override = default;

    void Init(const sf::Texture* texture, const sf::Vector2f& position) {
        SetPosition(position);
        SetTexture(*texture);
    }

    void AddWidget(IWidget* child) override {}
    void RemoveWidget(IWidget* child) override {}

    void SetPosition(const sf::Vector2f& position) override {
        m_position = position;
        m_sprite.setPosition(position);
    }
    void SetTexture(const sf::Texture& texture) {
        m_sprite.setTexture(texture);
        ChangeSize();
    }
    void SetScale(const sf::Vector2f& scale) {
        m_sprite.setScale(scale);
        ChangeSize();
    }
    void AddRotation(const float& rotation) {
        auto m_rotation = m_sprite.getRotation() + rotation;
        m_sprite.setRotation(m_rotation);
    }

    void OnUpdateWidget(const float& deltaTime) override {}
    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        if(isVisibled)
            renderTarget.draw(m_sprite);
    }

    void SetVisibled(bool visibled) override {
        isVisibled = visibled;
    }

private :
    sf::Sprite m_sprite;

    void ChangeSize()
    {
        m_size = { m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height };
    }
};


#endif //POPOSIBENGINE_SPRITE_HPP
