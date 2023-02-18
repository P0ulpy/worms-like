#pragma once

#include "EntitiesRegistry.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include <memory>

namespace Engine
{
    template<class T>
    T *EntitiesRegistry::AddComponentTo(EntityHandle entityHandle)
    {
        return static_cast<T*>(GetSystem<T>()
                ->Add(entityHandle));
    }

    template<class T>
    T *EntitiesRegistry::GetComponentOf(EntityHandle entityHandle)
    {
        return static_cast<T*>(GetSystem<T>()
            ->Get(entityHandle));
    }

    template<class T>
    bool EntitiesRegistry::HasComponent(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                ->Has(entityHandle);
    }

    template<class T>
    void EntitiesRegistry::RemoveComponentOf(EntityHandle entityHandle)
    {
        return GetSystem<T>()
                ->Remove(entityHandle);
    }

    template<class T>
    void EntitiesRegistry::View(ComponentSystem<T>::ViewCallback callback)
    {
        GetSystem<T>()
                ->View(callback);
    }

    template<class TSystem, class TComponent>
    void EntitiesRegistry::AddSystem()
    {
        auto newSystem = new TSystem();

        m_componentSystems.insert_or_assign(TComponent::getClassType(), std::unique_ptr<TSystem>(newSystem));

        if constexpr (requires (TComponent& c) { c.OnUpdate(0.0f); })
            m_updatableSystems.insert_or_assign(TComponent::getClassType(), newSystem);

        if constexpr (requires (TComponent& c, sf::RenderTarget& renderTarget) { c.OnRender(renderTarget); })
            m_renderableSystems.insert_or_assign(TComponent::getClassType(), newSystem);
    }

    template<class T>
    ComponentSystem<T>* EntitiesRegistry::GetSystem()
    {
        if (!m_componentSystems.contains(T::getClassType()))
            AddSystem<ComponentSystem<T>, T>();

        return static_cast<ComponentSystem<T>*>(m_componentSystems[T::getClassType()].get());
    }
}