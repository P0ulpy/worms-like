//
// Created by Flo on 18/01/2023.
//

#ifndef POPOSIBENGINE_SPRITERENDERER_HPP
#define POPOSIBENGINE_SPRITERENDERER_HPP

#include "SFML/Graphics.hpp"

#include "../../ECS/Component/Component.hpp"
#include "Transform.hpp"
// TODO : delete me, i'm for tests purposes
#include "../Application/EngineApplication.hpp"

namespace Engine {

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
            // TODO : delete me, i'm for tests purposes
            transform = EngineApplication::Get()
                    ->GetScenesLayer()
                    .GetActiveScene()
                    ->GetEntitiesRegistry()
                    .GetComponentOf<Transform>(GetEntity().GetHandle());
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
