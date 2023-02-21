//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

#include "../../Core/UUID.hpp"
#include "../../RTTI/ClassType.hpp"
#include "../Handles/Handles.hpp"
#include "../Entity/Entity.hpp"

namespace Engine
{
    class Component : public IClassType
    {
    public:
        DECLARE_CLASS_TYPE(Component, RTTI::NoClassTypeAncestor)

    public:
        Component() = default;

        /* Implementable Component Events
        void OnAwake();
        void OnStart();
        void OnUpdate(const float& deltaTime);
        void OnRender(sf::RenderTarget &renderTarget);
        void OnImGuiRender();
        void OnDestroy();
        */

        virtual void SetActive(bool active)     { m_active = active; }
        [[nodiscard]] bool IsActive() const     { return m_active; }
        [[nodiscard]] Entity& GetEntity() /* const */ { m_entity = Entity(m_entityHandle, m_scene); return m_entity; }
        [[nodiscard]] Scene* GetScene() const { return m_scene; }

        [[nodiscard]] inline EntityHandle GetHandle() const { return m_handle; }

        operator bool() const               { return m_handle != ComponentHandle::Null; }
        operator ComponentHandle() const    { return m_handle; }
        operator uint64_t() const           { return (uint64_t)m_handle; }

        bool operator==(const Component& other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const Component& other) const
        {
            return !(*this == other);
        }

    private:
        bool m_active = true;
        ComponentHandle m_handle = ComponentHandle::Null;
        EntityHandle m_entityHandle = EntityHandle::Null;
        Entity m_entity { EntityHandle::Null, nullptr };

        // Component can not exist if his scene is destroyed so a raw pointer is fine here
        Scene* m_scene = nullptr;

        template <class TComponent>
        friend class ComponentSystem;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
