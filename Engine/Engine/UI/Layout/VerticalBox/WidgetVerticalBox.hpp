//
// Created by Flo on 22/02/2023.
//

#pragma once

#include "../../Widget/Widget.hpp"

namespace Engine::UI {

        class WidgetVerticalBox : public Widget {
        public:
            DECLARE_CLASS_TYPE(WidgetVerticalBox, Widget)

            void Init(const sf::Vector2f& position, const float spacing);

        private:
            void UpdatePosition() const override;
            void UpdateSize() const override;

            float m_spacing { 0.0f };
        };
} // UI
