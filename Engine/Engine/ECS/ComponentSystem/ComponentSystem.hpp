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

} // Engine

#include "ComponentSystem.tpp"

#endif //POPOSIBENGINE_COMPONENTSYSTEM_HPP
