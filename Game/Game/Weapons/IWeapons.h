//
// Created by 33644 on 02/03/2023.
//

#pragma once

#include "Engine/ECS/Component/Component.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace Game::Actors {
    class PlayerCharacter;
}

namespace Game::Weapons
{
    class Weapons : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(Weapons, Engine::Component)

        Weapons() = default;

        virtual void OnUpdate(const float& DeltaTime) {};
        virtual void OnRender(sf::RenderTarget& renderTarget) { renderTarget.draw(m_sprite); };

        virtual void OnAwake() {};

        void SetTexture(const sf::Texture* texturePath) { m_sprite.setTexture(*texturePath); };

        virtual void Activate() {};

        [[nodiscard]] float GetDamage() const { return m_damage; }

        Game::Actors::PlayerCharacter* Owner = nullptr;
    protected:
        sf::Sprite m_sprite;
        float m_damage = 0.f;
    };
}