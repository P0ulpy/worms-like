//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_SCENE_HPP
#define PATHFINDER_SCENE_HPP

#include "SFML/Graphics/RenderTarget.hpp"

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

        static void SetActiveScene(Scene* scene) { s_activeScene = scene; }
        [[nodiscard]] static Scene* GetActiveScene() { return s_activeScene; }

        [[nodiscard]] EntitiesRegistry& GetEntitiesRegistry() { return m_registry; }

    private:
        static Scene* s_activeScene;
        EntitiesRegistry m_registry;
    };

    template<typename T>
    T *Entity::AddComponent()
    {
        auto* scene = Scene::GetActiveScene();
        return scene->GetEntitiesRegistry().AddComponentTo<T>(m_handle);
    }

    template<typename T>
    T *Entity::GetComponent()
    {
        auto* scene = Scene::GetActiveScene();
        return scene->GetEntitiesRegistry().GetComponentOf<T>(m_handle);
    }

    template<typename T>
    bool Entity::HasComponent()
    {
        auto* scene = Scene::GetActiveScene();
        return scene->GetEntitiesRegistry().HasComponent<T>(m_handle);
    }

    template<typename T>
    void Entity::RemoveComponent()
    {
        auto* scene = Scene::GetActiveScene();
        scene->GetEntitiesRegistry().RemoveComponentOf<T>(m_handle);
    }

} // Engine

#endif //PATHFINDER_SCENE_HPP
