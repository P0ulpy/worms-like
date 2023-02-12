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

        template<typename T>
        T* AddComponent();

        template<typename T>
        T* GetComponent();

        template<typename T>
        bool HasComponent();

        template<typename T>
        void RemoveComponent();

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

    private:
        EntityHandle m_handle = EntityHandle::Null;

        friend class Component;
    };

} // Engine

#endif //POPOSIBENGINE_ENTITY_HPP
