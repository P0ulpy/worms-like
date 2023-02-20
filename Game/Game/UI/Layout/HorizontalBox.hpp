//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_HORIZONTALBOX_HPP
#define POPOSIBENGINE_HORIZONTALBOX_HPP


#include "../WidgetComposite.hpp"

class HorizontalBox : public WidgetComposite {
public :
    explicit HorizontalBox()
        : m_spacing(0)
    {}

    void SetSpacing(float spacing) {
        m_spacing = spacing;
    }

private:
    void UpdateWidgetPosition() override {
        float x = m_position.x;
        for (auto& child : m_children) {
            child->SetPosition(x, m_position.y);
            x += child->GetSize().x + m_spacing;
        }
    }

    float m_spacing;
};


#endif //POPOSIBENGINE_HORIZONTALBOX_HPP
