//
// Created by Flo on 15/12/2022.
//

#ifndef PATHFINDER_ICOMPONENTSYSTEM_HPP
#define PATHFINDER_ICOMPONENTSYSTEM_HPP

#include <functional>

#include "../Component/Component.hpp"

namespace Engine
{
    class IComponentSystem
    {
    public:
        virtual ~IComponentSystem() = default;

        virtual Component* Add(EntityHandle entityHandle) = 0;
        virtual void Remove(EntityHandle entityHandle) = 0;
        virtual bool Has(EntityHandle entityHandle) = 0;
        virtual Component* Get(EntityHandle entityHandle) = 0;

        virtual void DispatchAwake() = 0;
        virtual void DispatchStart() = 0;
        virtual void DispatchUpdate(const float& deltaTime) = 0;
        virtual void DispatchRender(sf::RenderTarget &renderTarget) = 0;
        virtual void DispatchDestroy() = 0;

        using ViewCallback = std::function<void(Component*)>;

        virtual void View(ViewCallback callback) = 0;
    };
}

#endif //PATHFINDER_ICOMPONENTSYSTEM_HPP
