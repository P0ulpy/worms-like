//
// Created by Flo on 30/01/2023.
//

#ifndef POPOSIBENGINE_ENTITY_HPP
#define POPOSIBENGINE_ENTITY_HPP

#include "../Handles/Handles.hpp"

namespace Engine {

    class Entity
    {
    public:
        Entity() = default;
        Entity(EntityHandle handle);
        Entity(const Entity& other) = default;

        /*template<typename T>
        T* AddComponent();

        template<typename T>
        T* GetComponent();

        template<typename T>
        bool HasComponent();

        template<typename T>
        void RemoveComponent();*/

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

    private:
        EntityHandle m_handle = EntityHandle::Null;

        friend class Component;
    };

    /*template<typename T>
    T *Entity::AddComponent()
    {
        return m_Scene->GetEntitiesRegistry().AddComponentTo<T>(m_handle);
    }

    template<typename T>
    T *Entity::GetComponent() {
        return m_Scene->GetEntitiesRegistry().GetComponentOf<T>(m_handle);
    }

    template<typename T>
    bool Entity::HasComponent() {
        return m_Scene->GetEntitiesRegistry().HasComponent<T>(m_handle);
    }

    template<typename T>
    void Entity::RemoveComponent() {
        m_Scene->GetEntitiesRegistry().RemoveComponentOf<T>(m_handle);
    }*/

} // Engine

#endif //POPOSIBENGINE_ENTITY_HPP
