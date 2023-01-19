#include "Entity.hpp"
#include "../../Core/Scene/Scene.hpp"

namespace Engine
{
    template<typename T>
    T *Entity::AddComponent() {
        return m_Scene->m_registry.AddComponentTo<T>(m_handle);
    }

    template<typename T>
    T *Entity::GetComponent() {
        return m_Scene->m_registry.GetComponentOf<T>(m_handle);
    }

    template<typename T>
    bool Entity::HasComponent() {
        return m_Scene->m_registry.HasComponent<T>(m_handle);
    }

    template<typename T>
    void Entity::RemoveComponent() {
        m_Scene->m_registry.RemoveComponentOf<T>(m_handle);
    }
}