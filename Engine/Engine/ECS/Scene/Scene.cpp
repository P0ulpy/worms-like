//
// Created by Flo on 25/11/2022.
//

#include "Scene.hpp"

namespace Engine
{
    Entity Scene::CreateEntity()
    {
        Entity entity = { m_registry.CreateEntity() };
        return entity;
    }

    void Scene::DestroyEntity(const Entity& entity) { DestroyEntity(entity.GetHandle()); }
    void Scene::DestroyEntity(const EntityHandle& entityHandle)
    {
        m_registry.DestroyEntity(entityHandle);
    }

    void Scene::OnStart() {

    }

    void Scene::OnUpdate(Timestep ts)
    {
        m_registry.UpdateAllUpdatable(ts);
    }

    void Scene::OnStop() {

    }

    void Scene::RenderScene(sf::RenderTarget &renderTarget)
    {
        m_registry.RenderAllRenderer(renderTarget);
    }

} // Engine