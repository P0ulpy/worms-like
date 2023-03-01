//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_SCENE_HPP
#define PATHFINDER_SCENE_HPP

#include "SFML/Graphics/RenderTarget.hpp"

#include "../../Core/Time.hpp"

#include "../Entity/Entity.hpp"
#include "../EntitiesRegistry/EntitiesRegistry.hpp"
#include "../../Core/Camera/Camera.hpp"

namespace Engine
{
    namespace Physics {
        class IPhysicsSimulator;
    }
    class Scene
    {
    public:
        Scene() = default;
        ~Scene();

        void OnStart();
        void OnUpdate(Timestep DeltaTime);
        void OnStop();

        void Clear();

        void RenderScene(sf::RenderTarget& renderTarget);

        Entity CreateEntity();

        template <class TPrefab>
        Entity InstantiatePrefab();

        void DestroyEntity(const Entity& entity);
        void DestroyEntity(const EntityHandle& entity);

        template<class T>
        T* GetComponentOf(const EntityHandle& entityHandle);

        template<class T>
        T* AddComponentTo(const EntityHandle& entityHandle);

        template<class T>
        bool HasComponentIn(const EntityHandle& entityHandle);

        template<class T>
        void RemoveComponentOf(const EntityHandle& entityHandle);

        inline Engine::Camera::ICamera* GetActiveCamera() { return m_ActiveCamera; }
        inline void SetActiveCamera(Engine::Camera::ICamera* Camera) { m_ActiveCamera = Camera; }
        void AddPhysicsSimulator(Engine::Physics::IPhysicsSimulator* Simulator);
        void RemovePhysicsSimulator(Engine::Physics::IPhysicsSimulator *Simulator);
    private:
        EntitiesRegistry m_registry;
        Engine::Camera::ICamera* m_ActiveCamera = nullptr;
        std::unordered_map<RTTI::ClassType*, Engine::Physics::IPhysicsSimulator*> m_PhysicsSimulators;
    };

    // Scene

    template<class TPrefab>
    Entity Scene::InstantiatePrefab()
    {
        TPrefab prefab;
        return prefab.Instantiate(this);
    }

    template<class T>
    T *Scene::GetComponentOf(const EntityHandle &entityHandle) {
        return m_registry.GetComponentOf<T>(entityHandle);
    }

    template<class T>
    T *Scene::AddComponentTo(const EntityHandle &entityHandle)
    {
        return m_registry.AddComponentTo<T>(entityHandle);
    }

    template<class T>
    bool Scene::HasComponentIn(const EntityHandle &entityHandle)
    {
        return m_registry.HasComponent<T>(entityHandle);
    }

    template<class T>
    void Scene::RemoveComponentOf(const EntityHandle &entityHandle)
    {
        m_registry.RemoveComponentOf<T>(entityHandle);
    }

    // Entity

    template<class T>
    T* Entity::AddComponent()
    {
        return m_scene->AddComponentTo<T>(m_handle);
    }

    template<class T>
    T* Entity::GetComponent()
    {
        return m_scene->GetComponentOf<T>(m_handle);
    }

    template<class T>
    const T* Entity::GetComponent() const
    {
        return m_scene->GetComponentOf<T>(m_handle);
    }

    template<class T>
    bool Entity::HasComponent()
    {
        return m_scene->HasComponentIn<T>(m_handle);
    }

    template<class T>
    void Entity::RemoveComponent()
    {
        m_scene->RemoveComponentOf<T>(m_handle);
    }

} // Engine

#endif //PATHFINDER_SCENE_HPP
