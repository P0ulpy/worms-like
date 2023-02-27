//
// Created by Flo on 18/01/2023.
//
#include "../../Core/Logger/Logger.hpp"

namespace Engine
{
    template<class TComponent>
    Component *ComponentSystem<TComponent>::Add(EntityHandle entityHandle)
    {
        if(components.contains(entityHandle))
        {
            Logger::Err("this entity already have a (", TComponent::getClassType()->getTypeName(), ") allocated for him");
            return nullptr;
        }

        components.insert_or_assign(entityHandle, TComponent());
        components[entityHandle].m_handle = Core::UUID::CreateNew();
        components[entityHandle].m_entityHandle = entityHandle;

        if constexpr (requires (TComponent& c) { c.OnAwake(); })
            components[entityHandle].OnAwake();

        return static_cast<Component*>(&components[entityHandle]);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle)
    {
        typename std::unordered_map<EntityHandle, TComponent>::iterator itr;
        Remove(entityHandle, itr);
    }

    template<class TComponent>
    void ComponentSystem<TComponent>::Remove(EntityHandle entityHandle, typename std::unordered_map<EntityHandle, TComponent>::iterator& removedComponentIt)
    {
        if constexpr (requires (TComponent& c) { c.OnDestroy(); })
            components[entityHandle].OnDestroy();

        // Remove from his parent this component if TComponent extends from CompositeComponent
        // NOTE : (maybe use std::is_base_of_v instead of requires)
        if constexpr (requires (TComponent& c) { c.GetParent(); c.GetChildren(); })
        {
            for(auto& child : components[entityHandle].GetChildren())
                child->SetParent(nullptr);

            auto* parent = components[entityHandle].GetParent();
            if(parent)
            {
                parent->RemoveChild(&components[entityHandle]);
            }
        }

        auto toRemove = components.find(entityHandle);
        removedComponentIt = components.erase(toRemove);
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
    void ComponentSystem<TComponent>::Clear()
    {
        for (auto it = components.begin(); it != components.end(); )
        {
            Remove(it->first, it);
        }
    }

    template<class TComponent>
    ComponentSystem<TComponent>::~ComponentSystem()
    {
        Clear();
    }


    // Dispatch

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
                if(component.m_active)
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
                if(component.m_active)
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