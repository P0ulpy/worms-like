//
// Created by Flo on 18/01/2023.
//

#ifndef POPOSIBENGINE_SPRITERENDERER_HPP
#define POPOSIBENGINE_SPRITERENDERER_HPP

#include "../../ECS/Component/Component.hpp"

// TODO : delete me, i'm for tests purposes
#include <SFML/Graphics.hpp>
#include "Transform.hpp"

namespace Engine {

    class SpriteRenderer : public Engine::Component
    {
    public:
        DECLARE_RTTI(SpriteRenderer, Engine::Component)

        sf::Sprite RenderSprite;
        sf::Texture* Texture = nullptr;
        Transform* transform = nullptr;

        void Init(sf::Texture* texture)
        {
            Texture = texture;
            RenderSprite.setTexture(*Texture);
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
                RenderSprite.setPosition(transform->Pos);
                RenderSprite.setRotation(transform->Angle);
                RenderSprite.setScale(transform->Scale);
            }
        }
    };

} // Engine

#endif //POPOSIBENGINE_SPRITERENDERER_HPP
