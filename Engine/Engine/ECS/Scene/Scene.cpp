//
// Created by Flo on 25/11/2022.
//

#include "Scene.hpp"

namespace Engine
{
    Entity Scene::CreateEntity()
    {
        Entity entity = { m_registry.CreateEntity(), this };
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
        m_registry.HandleCollisions();
    }

    void Scene::OnStop()
    {

    }

    void Scene::RenderScene(sf::RenderTarget &renderTarget)
    {
        m_registry.RenderAllRenderer(renderTarget);
    }

    // NOTE : le problème principal de cette implémentation de destruction est global à toutes les méthodes de destruction des composants
    // qui se trouve dans l'Engine : les pointeurs vers les composants récupérés via GetComponentOf deviennes invalides
    // soucis qui pourrait être résolu en utilisant un smart pointer semblable à weak_ptr mais pour des std::unique_ptr
    // il faut également revoir le système d'attribution de Handles afin de libérer les handles lors de la destruction d'une entité ou d'un composant
    void Scene::Clear()
    {
        m_registry.DestroyAll();
    }

} // Engine