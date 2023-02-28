//
// Created by Flo on 27/02/2023.
//

#pragma once

#include "../../ECS/Entity/Entity.hpp"
#include "../../ECS/Scene/Scene.hpp"

namespace Engine
{
    template <class... TComponents>
    class EntityPrefab
    {
    public:
        [[nodiscard]] Entity Instantiate(Scene* scene);
        virtual void Init(Entity& entity) = 0;

    private:
        template<class TComponent, class... TComponentsToInstantiate>
        void InstantiateComponents(Entity& entity);
    };

    template<class... TComponents>
    Entity EntityPrefab<TComponents...>::Instantiate(Scene* scene)
    {
        Entity entity = scene->CreateEntity();
        InstantiateComponents<TComponents...>(entity);
        Init(entity);

        return entity;
    }

    template<class... TComponents>
    template<class TComponent, class... TComponentsToInstantiate>
    void EntityPrefab<TComponents...>::InstantiateComponents(Entity &entity)
    {
        if constexpr (sizeof...(TComponentsToInstantiate) > 0)
        {
            entity.AddComponent<TComponent>();
            InstantiateComponents<TComponentsToInstantiate...>(entity);
        }
    }

} // Engine
