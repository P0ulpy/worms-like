//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_GRID_HPP
#define POPOSIBENGINE_GRID_HPP


#include "../WidgetComposite.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Grid : public WidgetComposite {
public:
    explicit Grid() = default;

    void OnRenderWidget(sf::RenderTarget& renderTarget) override
    {
/*      float x = m_position.x;
        float y = m_position.y;

        for(int row = 0; row < m_rows; row++) {
            for(int column = 0; column < m_columns; column++) {
                sf::RectangleShape rect;
                rect.setPosition({x, y});
                rect.setSize(sf::Vector2f(m_cellWidth, m_cellHeight));
                rect.setFillColor(sf::Color(169,169,169 , 100)); //Gray
                //rect.setOutlineColor(sf::Color::Red);
                //rect.setOutlineThickness(1);
                renderTarget.draw(rect);

                x += m_cellWidth + m_spacing;
            }
            x = m_position.x;
            y += m_cellHeight + m_spacing;
        }*/

        for(auto& child : m_children) {
            child->OnRenderWidget(renderTarget);
        }
    }

    void SetSpacing(float spacing) { m_spacing = spacing; }

    void SetGridSize(int rows, int columns)
    {
        m_columns = columns;
        m_rows = rows;
    }

    void SetCellSize(float width, float height)
    {
        m_cellWidth = width;
        m_cellHeight = height;
    }

private:
    int m_columns = 0;
    int m_rows = 0;

    float m_cellWidth = 0;
    float m_cellHeight = 0;

    float m_spacing = 0;

    void UpdateWidgetPosition() override {
        float x = m_position.x;
        float y = m_position.y;

        int column = 0;

        if(m_children.empty())
            return;

        if(m_cellWidth == 0 || m_cellHeight == 0) {
            auto childSize = m_children[0]->GetSize();
            m_cellWidth = childSize.x;
            m_cellHeight = childSize.y;

            SetSize({
                (m_cellWidth * (float)m_columns) + m_spacing * (float)(m_columns - 1),
                (m_cellHeight * (float)m_rows) + m_spacing * (float)(m_rows - 1)
            });
        }

        for (auto& child : m_children) {

            auto childSize = child->GetSize();
            auto delta = sf::Vector2f(m_cellWidth - childSize.x, m_cellHeight - childSize.y);

            child->SetPosition(sf::Vector2f(x + delta.x / 2, y + delta.y / 2));
            x += m_cellWidth + m_spacing;
            column++;
            if(column == m_columns) {
                column = 0;
                x = m_position.x;
                y += m_cellHeight + m_spacing;
            }
        }
    }

};


#endif //POPOSIBENGINE_GRID_HPP
