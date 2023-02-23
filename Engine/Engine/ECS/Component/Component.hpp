//
// Created by Flo on 25/11/2022.
//

#ifndef PATHFINDER_COMPONENT_HPP
#define PATHFINDER_COMPONENT_HPP

#include "../../Core/UUID.hpp"
#include "../../Core/RTTI/ClassType.hpp"
#include "../Handles/Handles.hpp"
#include "../Entity/Entity.hpp"

namespace Engine
{
    class Component : public IClassType
    {
    public:
        DECLARE_CLASS_TYPE(Component, NoClassTypeAncestor)

    public:
        Component() = default;

        /* Implementable Component Events methods
        void OnAwake();
        void OnStart();
        void OnUpdate(const float& deltaTime);
        void OnRender(sf::RenderTarget &renderTarget);
        void OnImGuiRender();
        void OnDestroy();
        */

        virtual void SetActive(bool active)     { m_active = active; }
        [[nodiscard]] bool IsActive() const     { return m_active; }
        [[nodiscard]] Entity GetEntity() const  { return { m_entityHandle }; }

    private:
        bool m_active = true;
        ComponentHandle m_handle = ComponentHandle::Null;
        EntityHandle m_entityHandle = EntityHandle::Null;

        template <class TComponent>
        friend class ComponentSystem;
    };

} // Engine

#endif //PATHFINDER_COMPONENT_HPP
