#include "EntitiesRegistry.hpp"

#include <memory>

namespace Engine
{
    template<class T>
    T *EntitiesRegistry::AddComponentTo(EntityHandle entityHandle, Scene *scene)
    {
        return static_cast<T*>(GetSystem<T>()
                ->Add(entityHandle, scene));
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

    template<class TSystem, class T>
    void EntitiesRegistry::AddSystem()
    {
        // TODO : pre instanciate Core component systems to be close in memory
        m_componentSystems.insert_or_assign(T::getClassRTTI(), std::unique_ptr<TSystem>(new TSystem()));
    }

    template<class T>
    ComponentSystem<T>* EntitiesRegistry::GetSystem()
    {
        if (!m_componentSystems.contains(T::getClassRTTI()))
            AddSystem<ComponentSystem<T>, T>();

        return static_cast<ComponentSystem<T>*>(m_componentSystems[T::getClassRTTI()].get());
    }
}