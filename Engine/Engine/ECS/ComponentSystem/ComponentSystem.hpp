//
// Created by Flo on 18/01/2023.
//

#pragma once

#include "IComponentSystem.hpp"

#include "../../Core/Logger/Logger.hpp"
#include <unordered_map>

namespace Engine
{
    // TODO : use a custom memory pool instead of std::unordered_map to store components contiguously

    template <class TComponent>
    class ComponentSystem : public IComponentSystem
    {
        static_assert(std::is_base_of_v<Component, TComponent>, "Invalid TComponent type, a component must be derived from Engine::Component");

    public:
        ~ComponentSystem() override;

        std::unordered_map<EntityHandle, TComponent> components {};

        Component* Add(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle) override;
        void Remove(EntityHandle entityHandle, typename std::unordered_map<EntityHandle, TComponent>::iterator& removedComponentIt);
        bool Has(EntityHandle entityHandle) override;
        Component* Get(EntityHandle entityHandle) override;
        TComponent* GetOf(EntityHandle entityHandle);
        void View(ViewCallback callback) override;
        void Clear() override;

        void DispatchAwake() override;
        void DispatchStart() override;
        void DispatchUpdate(const float& deltaTime) override;
        void DispatchRender(sf::RenderTarget &renderTarget) override;
        void DispatchDestroy() override;
    };
} // Engine

#include "ComponentSystem.tpp"
