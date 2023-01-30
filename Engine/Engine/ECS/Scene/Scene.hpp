//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_SCENE_HPP
#define PATHFINDER_SCENE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../Core/Time.hpp"

#include "../Entity/Entity.hpp"
#include "../EntitiesRegistry/EntitiesRegistry.hpp"

namespace Engine
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        void OnStart();
        void OnUpdate(Timestep ts);
        void OnStop();

        void RenderScene(sf::RenderTarget& renderTarget);

        Entity CreateEntity();

        void DestroyEntity(const Entity& entity);
        void DestroyEntity(const EntityHandle& entity);

        //Entity GetEntityByHandle(const EntityHandle& handle);

        [[nodiscard]] EntitiesRegistry& GetEntitiesRegistry() { return m_registry; }

    private:
        EntitiesRegistry m_registry;
    };

} // Engine

#endif //PATHFINDER_SCENE_HPP
