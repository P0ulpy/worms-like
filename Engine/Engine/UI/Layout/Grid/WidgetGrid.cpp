//
// Created by Flo on 22/02/2023.
//

#include "WidgetGrid.hpp"

namespace Engine::UI {
        void WidgetGrid::Init(const sf::Vector2u &gridSize, const sf::Vector2f &gridSpacing, const sf::Vector2f &position, const sf::Vector2f &cellSize) {
            SetGridSize(gridSize);
            SetGridSpacing(gridSpacing);
            SetCellSize(cellSize);
            SetPosition(position);  // This will call UpdatePosition() and UpdateSize()
        }

        void WidgetGrid::SetGridSize(const sf::Vector2u &gridSize) {
            m_gridSize = gridSize;
            UpdateSize();
        }

        void WidgetGrid::SetGridSpacing(const sf::Vector2f &gridSpacing) {
            m_gridSpacing = gridSpacing;
            UpdateSize();
        }

        void WidgetGrid::SetCellSize(const sf::Vector2f &cellSize) {
            m_cellSize = cellSize;
            UpdateSize();
        }

        void WidgetGrid::UpdatePosition() const {
            auto children = GetChildren();

            if(children.empty())
                return;

            auto position = GetPosition();
            int column = 0;
            int line = 0;

            for (auto& child : children) {

                auto childSize = child->GetSize();
                auto delta = sf::Vector2f(m_cellSize.x - childSize.x, m_cellSize.y - childSize.y);

                child->SetPosition(sf::Vector2f(position.x + delta.x / 2, position.y + delta.y / 2));
                position.x += m_cellSize.x + m_gridSpacing.x;
                column++;
                if(column == m_gridSize.x) {
                    column = 0;
                    position.x = GetPosition().x;
                    position.y += m_cellSize.y + m_gridSpacing.y;
                    line++;

                    if(line == m_gridSize.y)
                        break;
                }
            }

        }

        void WidgetGrid::UpdateSize() const {
            m_size = sf::Vector2f {
                    m_cellSize.x * m_gridSize.x + m_gridSpacing.x * (m_gridSize.x - 1),
                    m_cellSize.y * m_gridSize.y + m_gridSpacing.y * (m_gridSize.y - 1)
            };
        }
} // UI