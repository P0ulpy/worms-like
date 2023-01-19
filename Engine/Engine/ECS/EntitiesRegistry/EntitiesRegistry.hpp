//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include "../Entity/Entity.hpp"
#include "../Component/Component.hpp"
#include "../../Core/Logger/Logger.hpp"
#include "ComponentSystem.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <unordered_map>
#include <map>
#include <stack>
#include <sstream>

namespace Engine
{
    /* Entities registry is the core interface for our ESC system
     * it manages Components association with Entities
     * */
    class EntitiesRegistry
    {
    public:

        EntityHandle CreateEntity();

        void DestroyEntity(EntityHandle handle);

        template<class T>
        T* AddComponentTo(EntityHandle entityHandle, Scene* scene);

        template<class T>
        T* GetComponentOf(EntityHandle entityHandle);

        template<class T>
        bool HasComponent(EntityHandle entityHandle);

        // TODO : found a way to notify Component destruction to all objects how reference it currently if a Component is removed accessing to his memory address don't throw any exception
        template<class T>
        void RemoveComponentOf(EntityHandle entityHandle);

        template<class T>
        void View(ComponentSystem<T>::ViewCallback callback);

        void StartAll();
        void UpdateAllUpdatable(const float& deltaTime);
        void RenderAllRenderer(sf::RenderTarget& renderTarget);

        template<class TSystem, class T>
        void AddSystem();

        template<class T>
        ComponentSystem<T>* GetSystem();

    private:
        std::unordered_map<RTTI*, std::unique_ptr<IComponentSystem>> m_componentSystems;

        void View(IComponentSystem::ViewCallback callback);
    };

} // Engine

#include "EntitiesRegistry.tpp"

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
