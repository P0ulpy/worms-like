//
// Created by Flo on 21/02/2023.
//

#pragma once

#include <vector>
#include <algorithm>
#include "../../ECS/Component/Component.hpp"

namespace Engine
{
    /** CompositeComponent is designed to add a composite logic through the ECS
     * CompositeComponent<TCompositeBaseClass> class is not designed to be used "as it", you should derive from it.
     * TCompositeBaseClass should be used for parents and children as a base class and must extends from CompositeComponent<TCompositeBaseClass>
     * */
    template <class TCompositeBaseClass>
    class CompositeComponent : public Component
    {
        // Check if TCompositeBaseClass extends from CompositeComponent<TCompositeBaseClass>
        //static_assert(std::is_base_of_v<Component, TCompositeBaseClass>, "Invalid TCompositeBaseClass type, a childComponent must be derived from Engine::CompositeComponent<TCompositeBaseClass>");

    public:
        DECLARE_CLASS_TYPE(CompositeComponent<TCompositeBaseClass>, Component)

        virtual void AddChild(TCompositeBaseClass* child);
        virtual void RemoveChild(TCompositeBaseClass* child) const;
        void SetActive(bool active) override;

        virtual void SetParent(TCompositeBaseClass* parent)             { m_parent = parent; };
        const TCompositeBaseClass* GetParent() const                    { return m_parent; };
        const std::vector<TCompositeBaseClass*>& GetChildren() const    { return m_children; };

    private:
        mutable TCompositeBaseClass* m_parent { nullptr };
        mutable std::vector<TCompositeBaseClass*> m_children;
    };

    template<class TCompositeBaseClass>
    void CompositeComponent<TCompositeBaseClass>::SetActive(bool active)
    {
        Component::SetActive(active);

        for(auto& child : m_children)
            child->SetActive(active);
    }

    template<class TCompositeBaseClass>
    void CompositeComponent<TCompositeBaseClass>::AddChild(TCompositeBaseClass* child)
    {
        child->SetParent(static_cast<TCompositeBaseClass*>(this));
        m_children.push_back(child);

        std::sort(m_children.begin(), m_children.end(), [](const auto& lhs, const auto& rhs) {
            return lhs->GetLayoutIndex() < rhs->GetLayoutIndex();
        });
    }

    template<class TCompositeBaseClass>
    void CompositeComponent<TCompositeBaseClass>::RemoveChild(TCompositeBaseClass* child) const
    {
        child->SetParent(nullptr);
        m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
    }
} // Engine