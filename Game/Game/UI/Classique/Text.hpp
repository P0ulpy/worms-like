//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_TEXT_HPP
#define POPOSIBENGINE_TEXT_HPP


#include "../IWidget.hpp"
#include "SFML/Graphics/Text.hpp"

class Text : public IWidget {
public :
    Text()
        : m_colorText(sf::Color::White)
    {
        m_text.setFillColor(m_colorText);
    }

    ~Text() override = default;

    void Init(const std::string& text, const sf::Font* font, const sf::Vector2f& position, const unsigned int& size, const sf::Color& color = sf::Color::White) {
        SetPosition(position);
        SetText(text);
        SetFont(*font);
        SetColor(color);
        SetCharacterSize(size);
        SetSize(sf::Vector2f{m_text.getGlobalBounds().width, m_text.getGlobalBounds().height});
    }

    void AddWidget(IWidget* child) override {}
    void RemoveWidget(IWidget* child) override {}

    void OnUpdateWidget(const float& deltaTime) override {}
    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        if(isVisibled)
            renderTarget.draw(m_text);
    }

    void SetPosition(const sf::Vector2f& position) override {
        m_position = position;
        m_text.setPosition(position);
    }

    void AddRotation(const float& rotation) {
        auto m_rotation = m_text.getRotation() + rotation;
        m_text.setRotation(m_rotation);
    }

    void SetSize(const sf::Vector2f& size) override {
        m_size = size;
    }

    void SetText(const std::string& text) {
        m_text.setString(text);
    }
    void SetFont(const sf::Font& font) {
        m_text.setFont(font);
    }
    void SetColor(const sf::Color& color) {
        m_colorText = color;
        m_text.setFillColor(color);
    }
    void SetCharacterSize(const unsigned int& size) {
        m_text.setCharacterSize(size);
    }

    void SetVisibled(bool visibled) override {
        isVisibled = visibled;
        m_text.setFillColor(visibled ? m_colorText : sf::Color::Transparent);
    }

    [[nodiscard]] const sf::Text& GetText() const { return m_text; }

private:
    sf::Text m_text;
    sf::Color m_colorText;
};


#endif //POPOSIBENGINE_TEXT_HPP
