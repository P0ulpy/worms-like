//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_IWIDGET_HPP
#define POPOSIBENGINE_IWIDGET_HPP


#include "Engine/ECS/Component/Component.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class IWidget {

public:
    IWidget()
        : m_position(0, 0)
        , m_size(0, 0)
        , isVisibled(true)
    {}
    virtual ~IWidget() = default;

    virtual void AddWidget(IWidget* child) = 0;
    virtual void RemoveWidget(IWidget* child) = 0;

    virtual void OnUpdateWidget(const float& deltaTime) = 0;
    virtual void OnRenderWidget(sf::RenderTarget& renderTarget) = 0;

    void SetPosition(const float& x, const float& y) { SetPosition({x, y}); }
    virtual void SetPosition(const sf::Vector2f& position) { m_position = position; }
    void SetSize(const float& x, const float& y) { SetSize({x, y}); }
    virtual void SetSize(const sf::Vector2f& size) { m_size = size; }

    [[nodiscard]] const sf::Vector2f& GetPosition() const { return m_position; }
    [[nodiscard]] const sf::Vector2f& GetSize() const { return m_size; }

    virtual void SetVisibled(bool visibled) { isVisibled = visibled; }
    [[nodiscard]] bool IsVisibled() const { return isVisibled; }

protected:
    sf::Vector2f m_position;
    sf::Vector2f m_size;

    bool isVisibled;
};


#endif //POPOSIBENGINE_IWIDGET_HPP
