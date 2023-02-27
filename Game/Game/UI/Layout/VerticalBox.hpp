//
// Created by Admin on 20/02/2023.
//

#ifndef POPOSIBENGINE_VERTICALBOX_HPP
#define POPOSIBENGINE_VERTICALBOX_HPP


#include "../WidgetComposite.hpp"

class VerticalBox : public WidgetComposite {
public :
    explicit VerticalBox()
            : m_spacing(0)
    {}

    void SetSpacing(float spacing) {
        m_spacing = spacing;
    }

private:
    void UpdateWidgetPosition() override {
        float y = m_position.y;
        for (auto& child : m_children) {
            child->SetPosition(m_position.x, y);
            y += child->GetSize().y + m_spacing;
        }
    }

    float m_spacing;
};


#endif //POPOSIBENGINE_VERTICALBOX_HPP
