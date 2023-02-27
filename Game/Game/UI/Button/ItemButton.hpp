//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_ITEMBUTTON_HPP
#define POPOSIBENGINE_ITEMBUTTON_HPP


#include "SpriteButton.hpp"

enum class ItemSide {
    BottomLeft,
    BottomRight,
    TopLeft,
    TopRight
};

class ItemButton : public SpriteButton {
public:
    ItemButton()
    : m_itemSide(ItemSide::BottomRight)
    {}

    ~ItemButton() override
    {
        delete m_item;
    }

    void Init(const sf::Texture* texture, const sf::Vector2f& position, ItemSide itemSide = ItemSide::BottomRight) {
        SpriteButton::Init(texture, position);
        m_itemSide = itemSide;
        SetItemPosition();
    }

    void SetPosition(const sf::Vector2f &position) override {
        SpriteButton::SetPosition(position);
        if(m_item != nullptr)
            SetItemPosition();
    }

    void AddWidget(IWidget* child) override {
        m_item = child;
        SetItemPosition();
    }
    void RemoveWidget(IWidget* child) override {
        delete m_item;
        m_item = nullptr;
    }

    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        if(isVisibled) {
            SpriteButton::OnRenderWidget(renderTarget);
            if(m_item != nullptr)
                m_item->OnRenderWidget(renderTarget);
        }
    }

    void SetVisibled(bool visibled) override {
        isVisibled = visibled;
        if(m_item != nullptr)
            m_item->SetVisibled(visibled);
    }
private:
    IWidget* m_item = nullptr;
    ItemSide m_itemSide;

    void SetItemPosition()
    {
        if(m_item != nullptr) {
            switch (m_itemSide) {
                case ItemSide::BottomLeft:
                    m_item->SetPosition({m_position.x, m_position.y + m_size.y - m_item->GetSize().y});
                    break;
                case ItemSide::BottomRight:
                    m_item->SetPosition({m_position.x + m_size.x - m_item->GetSize().x, m_position.y + m_size.y - m_item->GetSize().y});
                    break;
                case ItemSide::TopLeft:
                    m_item->SetPosition({m_position.x - m_item->GetSize().x, m_position.y - m_item->GetSize().y});
                    break;
                case ItemSide::TopRight:
                    m_item->SetPosition({m_position.x + m_size.x - m_item->GetSize().x, m_position.y - m_item->GetSize().y});
                    break;
            }
        }
    }
};


#endif //POPOSIBENGINE_ITEMBUTTON_HPP
