//
// Created by Flo on 18/01/2023.
//

#pragma once

#include "IComponentSystem.hpp"

#include "../../Core/Logger/Logger.hpp"
#include <unordered_map>

namespace Engine
{
    // TODO : use a custom memory pool instead of std::unordered_map to store components contiguously

    template <class TComponent>
    class ComponentSystem : public IComponentSystem
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "Invalid TComponent type, a component must be derived from Engine::Component");

    public:
        std::unordered_map<EntityHandle, TComponent> components {};

        Component* Add(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle) override;
        bool Has(EntityHandle entityHandle) override;
        Component* Get(EntityHandle entityHandle) override;
        void View(ViewCallback callback) override;

        void DispatchAwake() override;
        void DispatchStart() override;
        void DispatchUpdate(const float& deltaTime) override;
        void DispatchRender(sf::RenderTarget &renderTarget) override;
        void DispatchDestroy() override;
    };

    template<class TComponent>
    Component* ComponentSystem<TComponent>::Add(EntityHandle entityHandle)
    {
        if(components.contains(entityHandle))
        {
            //Logger::Err("this entity already have a TComponent allocated for it");
            return nullptr;
        }

        components.insert_or_assign(entityHandle, TComponent());
        components[entityHandle].m_handle = Core::UUID::CreateNew();
        components[entityHandle].m_entityHandle = entityHandle;

        return static_cast<Component*>(&components[entityHandle]);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle)
    {
        components.erase(entityHandle);
    }

    template<class TComponent>
    bool ComponentSystem<TComponent>::Has(EntityHandle entityHandle)
    {
        return components.contains(entityHandle);
    }

    template<class TComponent>
    Component* ComponentSystem<TComponent>::Get(EntityHandle entityHandle)
    {
        return static_cast<Component*>(&components[entityHandle]);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::View(IComponentSystem::ViewCallback callback)
    {
        for(auto& [ handle, component ] : components)
        {
            callback(&component);
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchAwake()
    {
        if constexpr (requires (TComponent& c) { c.OnAwake(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnAwake();
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchStart()
    {
        if constexpr (requires (TComponent& c) { c.OnStart(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnStart();
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchUpdate(const float &deltaTime)
    {
        if constexpr (requires (TComponent& c) { c.OnUpdate(0.0f); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnUpdate(deltaTime);
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchRender(sf::RenderTarget &renderTarget)
    {
        if constexpr (requires (TComponent& c, sf::RenderTarget& rT) { c.OnRender(rT); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnRender(renderTarget);
            }
        }
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::DispatchDestroy()
    {
        if constexpr (requires (TComponent& c) { c.OnDestroy(); })
        {
            for(auto& [ handle, component ] : components)
            {
                component.OnDestroy();
            }
        }
    }
} // Engine

#include "ComponentSystem.tpp"
