//
// Created by Admin on 19/02/2023.
//

#pragma once

#include "IWidget.hpp"
#include <vector>

class WidgetComposite : public IWidget {
public:
    WidgetComposite() = default;
    ~WidgetComposite() override
    {
        for (auto& child : m_children) {
            delete child;
        }
    }

    void AddWidget(IWidget* child) override {
        m_children.push_back(child);
        UpdateWidgetPosition();
    }

    void RemoveWidget(IWidget* child) override {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
        UpdateWidgetPosition();
    }

    void SetPosition(const sf::Vector2f& position) override {
        m_position = position;
        UpdateWidgetPosition();
    }

    void OnUpdateWidget(const float& deltaTime) override {
        for (auto& child : m_children) {
            child->OnUpdateWidget(deltaTime);
        }
    }

    void OnRenderWidget(sf::RenderTarget& renderTarget) override {
        for (auto& child : m_children) {
            child->OnRenderWidget(renderTarget);
        }
    }

    void SetVisibled(bool visibled) override {
        isVisibled = visibled;
        for(auto& child : m_children) {
            child->SetVisibled(visibled);
        }
    }

protected:
    virtual void UpdateWidgetPosition() {}

    std::vector<IWidget*> m_children;
};
