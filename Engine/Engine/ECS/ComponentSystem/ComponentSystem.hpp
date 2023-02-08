//
// Created by Flo on 18/01/2023.
//

#ifndef POPOSIBENGINE_COMPONENTSYSTEM_HPP
#define POPOSIBENGINE_COMPONENTSYSTEM_HPP

#include "IComponentSystem.hpp"
#include "../../Core/Logger/Logger.hpp"
#include <map>

namespace Engine
{
    template <class TComponent>
    class ComponentEntry
    {

    };

    template <class TComponent>
    class ComponentSystem : public IComponentSystem
    {
    public:
        std::map<EntityHandle, TComponent> components {};

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
    Component *ComponentSystem<TComponent>::Add(EntityHandle entityHandle)
    {
        if(components.contains(entityHandle))
        {
            Logger::Err("this entity already have a TComponent allocated for him");
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

#endif //POPOSIBENGINE_COMPONENTSYSTEM_HPP
