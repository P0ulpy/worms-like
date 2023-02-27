//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_PROGRESSBAR_HPP
#define POPOSIBENGINE_PROGRESSBAR_HPP


#include "../IWidget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class ProgressBar : public IWidget {
public:
    ProgressBar() = default;
    ~ProgressBar() override = default;

    void AddWidget(IWidget* child) override {}
    void RemoveWidget(IWidget* child) override {}

    void OnUpdateWidget(const float& deltaTime) override
    {
        //For tests
        if (m_progress < m_maxProgress) {
            m_progress += deltaTime;
        }

        if (m_progress > m_maxProgress) {
            m_progress = m_maxProgress;
        }

        auto progress = m_progress / m_maxProgress;

        m_progressBar.setSize({m_size.x * progress, m_size.y});
    }
    void OnRenderWidget(sf::RenderTarget& renderTarget) override
    {
        renderTarget.draw(m_borderShape);
        renderTarget.draw(m_background);
        renderTarget.draw(m_progressBar);
    }

    void Init(const sf::Vector2f& position, const sf::Vector2f& size, const float& maxProgress, const sf::Color& progressColor, const sf::Color& backgroundColor, const float& border, const sf::Color& borderColor)
    {
        m_progressColor = progressColor;
        m_backgroundColor = backgroundColor;
        m_border = border;
        m_borderColor = borderColor;
        m_maxProgress = maxProgress;

        m_background.setFillColor(m_backgroundColor);
        m_progressBar.setFillColor(m_progressColor);
        m_borderShape.setFillColor(m_borderColor);
        m_borderShape.setOutlineColor(m_borderColor);
        m_borderShape.setOutlineThickness(m_border);

        SetPosition(position);
        SetSize(size);
    }

    void SetPosition(const sf::Vector2f& position) override
    {
        m_position = position;
        m_background.setPosition(m_position);
        m_progressBar.setPosition(m_position);
        m_borderShape.setPosition(m_position - sf::Vector2f(m_border, m_border));
    }

    void SetSize(const sf::Vector2f& size) override
    {
        m_size = size;
        m_background.setSize(m_size);
        m_progressBar.setSize({m_size.x * m_progress, m_size.y});
        m_borderShape.setSize({m_size.x + m_border * 2, m_size.y + m_border * 2});
    }

    void SetProgress(const float& progress) { m_progress = progress; }
    void SetProgressColor(const sf::Color& color) { m_progressColor = color; }
    void SetBackgroundColor(const sf::Color& color) { m_backgroundColor = color; }
    void SetBorder(const float& border) { m_border = border; }
    void SetBorderColor(const sf::Color& color) { m_borderColor = color; }

private:
    float m_progress = 0.0f;
    float m_maxProgress;
    sf::Color m_progressColor = sf::Color::Green;
    sf::Color m_backgroundColor = sf::Color::Black;
    sf::Color m_borderColor = sf::Color::White;
    float m_border = 0.0f;

    sf::RectangleShape m_background;
    sf::RectangleShape m_progressBar;
    sf::RectangleShape m_borderShape;
};


#endif //POPOSIBENGINE_PROGRESSBAR_HPP
