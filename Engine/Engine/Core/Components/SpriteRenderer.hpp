//
// Created by Flo on 18/01/2023.
//

#pragma once

#include <SFML/Graphics.hpp>

#include "../../ECS/Component/Component.hpp"
#include "Transform.hpp"

namespace Engine::Components {
    class SpriteRenderer : public Engine::Component
    {
    public:
        DECLARE_CLASS_TYPE(SpriteRenderer, Engine::Component)

        sf::Sprite RenderSprite;
        sf::Texture* Texture = nullptr;
        Transform* transform = nullptr;

        void Init(sf::Texture* texture)
        {
            Texture = texture;
            RenderSprite.setTexture(*Texture);
        }

        void OnAwake()
        {

        }

        void OnStart()
        {
            transform = GetEntity().GetComponent<Transform>();
        }

        void OnRender(sf::RenderTarget &renderTarget) const
        {
            renderTarget.draw(RenderSprite);
        }

        void OnUpdate(const float& deltaTime)
        {
            if(transform)
            {
                RenderSprite.setPosition({(float) transform->Pos.GetX(), (float) transform->Pos.GetY()});
                RenderSprite.setRotation(transform->Angle);
                RenderSprite.setScale({(float) transform->Scale.GetX(), (float) transform->Scale.GetY()});
            }
        }
    };

} // Engine
