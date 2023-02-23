//
// Created by Flo on 18/01/2023.
//

#pragma once

#include "IComponentSystem.hpp"

namespace Engine
{
    template <class TComponent>
    class ComponentSystem : public IComponentSystem
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "Invalid TComponent type, a component must be derived from Engine::Component");

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
} // Engine

#include "ComponentSystem.tpp"
