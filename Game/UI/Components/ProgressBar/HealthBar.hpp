//
// Created by Admin on 28/02/2023.
//

#ifndef WORMS_LIKE_HEALTHBAR_HPP
#define WORMS_LIKE_HEALTHBAR_HPP

#include "Engine/UI/Components/ProgressBar/ProgressBarWidget.hpp"

namespace Game::UI
{
    class HealthBar : public Engine::UI::ProgressBarWidget {
    public:
        DECLARE_CLASS_TYPE(HealthBar, Engine::UI::ProgressBarWidget)

        HealthBar() = default;
        ~HealthBar() override = default;

        void OnUpdate(Engine::Timestep ts) override;

        void Init(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Texture& backgroundTexture, const sf::Texture& fillTexture);
    };
}


#endif //WORMS_LIKE_HEALTHBAR_HPP
