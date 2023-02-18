//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_ENTITIESREGISTRY_HPP
#define PATHFINDER_ENTITIESREGISTRY_HPP

#include <unordered_map>
#include <map>
#include <stack>

#include "SFML/Graphics/RenderTarget.hpp"

#include "../ComponentSystem/ComponentSystem.hpp"
#include "../../Core/RTTI/ClassType.hpp"
#include "../../Core/Components/Physics.hpp"
#include "../../Maths/Graph.hpp"

namespace Engine
{
    namespace PhysicsCalculations
    {
        struct EntityKdTreeNodeContent
        {
            EntityKdTreeNodeContent(
                EntityHandle H,
                Engine::Components::Transform* TC,
                Engine::Components::Physics::RigidBody2DdComponent* BC
            ) : Handle(H), TransformComponent(TC), BodyComponent(BC) {};

            EntityHandle Handle;
            Engine::Components::Transform* TransformComponent;
            Engine::Components::Physics::RigidBody2DdComponent* BodyComponent;

            [[nodiscard]] const Engine::Components::Transform::PointT& GetKdTreePosition() const
            {
                return TransformComponent->Pos;
            }

            bool operator==(const EntityKdTreeNodeContent& Other) const
            {
                return Handle == Other.Handle;
            }
        };

        using EntityKdTreeNode = Maths::Graph::BinaryTreeNode<EntityKdTreeNodeContent>;
        using EntityKdTree = Maths::Graph::KdTree<EntityKdTreeNode>;
    }

    /* Entities registry is the core interface for our ESC system
     * it manages Components association with Entities
     * */
    class EntitiesRegistry
    {
    public:
        EntityHandle CreateEntity();

        void DestroyEntity(EntityHandle handle);

        template<class T>
        T* AddComponentTo(EntityHandle entityHandle);

        template<class T>
        T* GetComponentOf(EntityHandle entityHandle);

        template<class T>
        bool HasComponent(EntityHandle entityHandle);

        // TODO : found a way to notify Component destruction to all objects how reference it currently if a Component is removed accessing to his memory address don't throw any exception
        template<class T>
        void RemoveComponentOf(EntityHandle entityHandle);

        template<class T>
        void View(ComponentSystem<T>::ViewCallback callback);

        void AwakeAll();
        void StartAll();
        void UpdateAllUpdatable(const float& deltaTime);
        void HandleCollisions();
        void RenderAllRenderer(sf::RenderTarget& renderTarget);

        template<class TSystem, class T>
        void AddSystem();

        template<class T>
        ComponentSystem<T>* GetSystem();

        void DestroyAll();

    private:
        bool m_markedForDestruction = false;

        std::unordered_map<ClassType*, std::unique_ptr<IComponentSystem>> m_componentSystems;

        std::unordered_map<ClassType*, IComponentSystem*> m_updatableSystems;
        std::unordered_map<ClassType*, IComponentSystem*> m_renderableSystems;

        void View(IComponentSystem::ViewCallback callback);
    };

} // Engine

#include "EntitiesRegistry.tpp"

#endif //PATHFINDER_ENTITIESREGISTRY_HPP
