//
// Created by Flo on 26/11/2022.
//

#include "EntitiesRegistry.hpp"
#include "../../Core/Components/Physics.hpp"
#include "../../Maths/Collisions.hpp"

namespace Engine
{
    EntityHandle EntitiesRegistry::CreateEntity()
    {
        return EntityHandle(Core::UUID::CreateNew());
    }

    void EntitiesRegistry::DestroyEntity(EntityHandle handle)
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            if(system->Has(handle))
            {
                system->Remove(handle);
            }
        }
    }

    void EntitiesRegistry::AwakeAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchAwake();
        }
    }

    void EntitiesRegistry::StartAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchStart();
        }
    }

    void EntitiesRegistry::UpdateAllUpdatable(const float& deltaTime)
    {
        for(auto& [classType, system] : m_updatableSystems)
        {
            system->DispatchUpdate(deltaTime);
        }
    }

    // @todo maybe do this only on moving entities?
    void EntitiesRegistry::HandleCollisions()
    {
        struct CollisionsHandlerUtil
        {
            using BodyComponentT = Engine::Components::Physics::RigidBody2DdComponent;
            mutable Engine::ComponentSystem<BodyComponentT>* ComponentSystem;
            const Engine::PhysicsCalculations::EntityKdTreeNode* NodeUnderCheck;
            mutable BodyComponentT* BodyNodeComponent;
            mutable Engine::Components::Transform* TransformNodeComponent;
            mutable std::vector<BodyComponentT::GeometricPoint2DT> NodeProjectablePoints;
            mutable std::vector<BodyComponentT::GeometricVector2DT> NodeAxes;

            CollisionsHandlerUtil(
                Engine::ComponentSystem<BodyComponentT>* System,
                const Engine::PhysicsCalculations::EntityKdTreeNode* Node
            ) : ComponentSystem(System), NodeUnderCheck(Node)
            {
                BodyNodeComponent = System->GetOf(Node->GetContent().Handle);
                TransformNodeComponent = BodyNodeComponent->GetEntity().GetComponent<Engine::Components::Transform>();
                ResetNodeAxesAndPoints();
            }

            void ResetNodeAxesAndPoints() const
            {
                NodeProjectablePoints = BodyNodeComponent->GetProjectablePoints();
                NodeAxes = BodyNodeComponent->GetAxes();
            }

            void ExecuteOnNodes(const PhysicsCalculations::EntityKdTreeNode* FromNode) const
            {
                if (nullptr != FromNode->GetLeftChild()) {
                    CheckAndHandleCollision(FromNode->GetLeftChild()->GetContent().Handle);
                    ExecuteOnNodes(FromNode->GetLeftChild());
                }
                if (nullptr != FromNode->GetRightChild()) {
                    CheckAndHandleCollision(FromNode->GetRightChild()->GetContent().Handle);
                    ExecuteOnNodes(FromNode->GetRightChild());
                }
            }

            void CheckAndHandleCollision(EntityHandle WithHandle) const
            {
                if (WithHandle == NodeUnderCheck->GetContent().Handle)
                    return;

                auto WithBodyComponent = ComponentSystem->GetOf(WithHandle);
                auto Axes = WithBodyComponent->GetAxes();
                Axes.insert(Axes.end(), NodeAxes.begin(), NodeAxes.end());
                const auto ProjectablePoints = WithBodyComponent->GetProjectablePoints();
                //
                if (Maths::Collisions::SAT::CheckOverlap(Axes, NodeProjectablePoints, ProjectablePoints)) {
                    // tmp
                    for (auto& Body : BodyNodeComponent->GetRigidBodies())
                    {
                        for (auto & Value : Body->Force.Values)
                        {
                            Value = 0;
                        }
                        for (auto & Value : Body->LinearVelocity.Values)
                        {
                            Value = 0;
                        }
                    }
                    BodyNodeComponent->HasGravity = false;
                    ResetNodeAxesAndPoints();
                }
            }
        };

        auto RigidBodyComponentSystem = GetSystem<Engine::Components::Physics::RigidBody2DdComponent>();

        std::array<std::vector<Engine::PhysicsCalculations::EntityKdTreeNodeContent>, 2> ToBeOrdered = {};
        for (auto& [handle, component] : RigidBodyComponentSystem->components) {
            if (!component.HasCollisions)
            {
                continue;
            }
            for (size_t i = 0; i < 2; i++)
            {
                ToBeOrdered[i].emplace_back(
                    handle,
                    component.GetEntity().GetComponent<Engine::Components::Transform>(),
                    &component
                );
            }
        }
        Engine::PhysicsCalculations::EntityKdTree KdTree;
        KdTree.CreateKdTree(ToBeOrdered);

        for(auto& [handle, component] : RigidBodyComponentSystem->components)
        {
            if (!component.HasCollisions)
            {
                continue;
            }

            Engine::PhysicsCalculations::EntityKdTreeNodeContent ExpectedNodeContent = {
                handle,
                component.GetEntity().GetComponent<Engine::Components::Transform>(),
                &component
            };
            auto Node = KdTree.FindNode<2>(ExpectedNodeContent);
            auto NodeTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
            const auto MaxSize = component.ComputeMaxDistanceFromTransform();

            auto CollisionsHandler = CollisionsHandlerUtil(
                RigidBodyComponentSystem,
                Node
            );
            PhysicsCalculations::EntityKdTree::NodeType::ConstNodeTypePtr CurrentNode = Node;
            auto CurrentTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
            while (MaxSize < CurrentTransformComponent->Pos.GetVector(NodeTransformComponent->Pos).GetLength())
            {
                CurrentNode = const_cast<const PhysicsCalculations::EntityKdTree::NodeType::ConstNodeTypePtr>(Node)->GetParent();
                CurrentTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
            }
            CollisionsHandler.ExecuteOnNodes(CurrentNode);
        }
    }

    void EntitiesRegistry::RenderAllRenderer(sf::RenderTarget& renderTarget)
    {
        for(auto& [classType, system] : m_renderableSystems)
        {
            system->DispatchRender(renderTarget);
        }
    }

    void EntitiesRegistry::DestroyAll()
    {
        m_updatableSystems.clear();
        m_renderableSystems.clear();
        m_componentSystems.clear();
    }

} // Engine