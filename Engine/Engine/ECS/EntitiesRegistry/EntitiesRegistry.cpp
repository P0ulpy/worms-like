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
        std::vector<EntityHandle> ToBeOrdered = {};
        for (auto& [handle, component] : GetSystem<Engine::Components::Physics::RigidBody2DdComponent>()->components) {
            if (!component.HasCollisions)
            {
                continue;
            }
            ToBeOrdered.push_back(handle);
        }
        PhysicsEntityKdTree.CreateKdTree<2, Maths::Point2D<double>, true>(
            ToBeOrdered,
            [this](EntityHandle handle)->Maths::Point2D<double> { return this->GetSystem<Engine::Components::Transform>()->GetOf(handle)->Pos; }
        );
//        struct CollisionsHandlerUtil
//        {
//            using BodyComponentT = Engine::Components::Physics::RigidBody2DdComponent;
//            mutable Engine::ComponentSystem<BodyComponentT>* ComponentSystem;
//            const Engine::PhysicsCalculations::EntityKdTreeNode* NodeUnderCheck;
//            mutable BodyComponentT* BodyNodeComponent;
//            mutable Engine::Components::Transform* TransformNodeComponent;
//
//            CollisionsHandlerUtil(
//                Engine::ComponentSystem<BodyComponentT>* System,
//                const Engine::PhysicsCalculations::EntityKdTreeNode* Node
//            ) : ComponentSystem(System), NodeUnderCheck(Node)
//            {
//                BodyNodeComponent = System->GetOf(Node->GetContent().Handle);
//                TransformNodeComponent = BodyNodeComponent->GetEntity().GetComponent<Engine::Components::Transform>();
//            }
//
//            void ExecuteOnNodes(const PhysicsCalculations::EntityKdTreeNode* FromNode) const
//            {
//                if (nullptr != FromNode->GetLeftChild()) {
//                    CheckAndHandleCollision(FromNode->GetLeftChild()->GetContent().Handle);
//                    ExecuteOnNodes(FromNode->GetLeftChild());
//                }
//                if (nullptr != FromNode->GetRightChild()) {
//                    CheckAndHandleCollision(FromNode->GetRightChild()->GetContent().Handle);
//                    ExecuteOnNodes(FromNode->GetRightChild());
//                }
//            }
//
//            void CheckAndHandleCollision(EntityHandle WithHandle) const
//            {
//                if (WithHandle == NodeUnderCheck->GetContent().Handle)
//                    return;
//
//                auto WithBodyComponent = ComponentSystem->GetOf(WithHandle);
//                auto WithBodyTransform = WithBodyComponent->GetEntity().GetComponent<Engine::Components::Transform>();
//                for (const auto& NodeBody : BodyNodeComponent->GetRigidBodies())
//                {
//                    for (const auto& WithBody : WithBodyComponent->GetRigidBodies())
//                    {
//                        std::vector<BodyComponentT::GeometricVector2DT> Axes = {};
//                        std::vector<BodyComponentT::GeometricPoint2DT> NodeProjectablePoints = {};
//                        std::vector<BodyComponentT::GeometricPoint2DT> ProjectablePoints = {};
//                        Maths::Collisions::SAT::ComputeAxesAndPointsToCheck<BodyComponentT::GeometricT, 2>(
//                            TransformNodeComponent->Scale,
//                            WithBodyTransform->Scale,
//                            TransformNodeComponent->Angle,
//                            WithBodyTransform->Angle,
//                            TransformNodeComponent->Pos,
//                            WithBodyTransform->Pos,
//                            NodeBody->GetBoundingBox(),
//                            WithBody->GetBoundingBox(),
//                            Axes,
//                            NodeProjectablePoints,
//                            ProjectablePoints
//                        );
//
//                        BodyComponentT::GeometricVector2DT OverlapResolvingVector;
//                        if (Maths::Collisions::SAT::CheckOverlap(
//                            Axes,
//                            NodeProjectablePoints,
//                            ProjectablePoints,
//                            OverlapResolvingVector
//                        )) {
//                            const std::function<void(
//                                Engine::Components::Transform*,
//                                BodyComponentT*,
//                                BodyComponentT::GeometricT
//                            )> HandleResolvingVectorOnComponent = [&OverlapResolvingVector](
//                                Engine::Components::Transform* TransformComponent,
//                                BodyComponentT* BodyComponent,
//                                BodyComponentT::GeometricT Modifier
//                            )->void
//                            {
//                                TransformComponent->Pos = TransformComponent->Pos + (OverlapResolvingVector * Modifier);
//                                // tmp
//                                for (auto& Body : BodyComponent->GetRigidBodies())
//                                {
//                                    Body->Force = BodyComponentT::PhysicsVector2DT{};
//                                    Body->LinearVelocity = BodyComponentT::PhysicsVector2DT{};
//                                }
//                            };
//                            if (BodyNodeComponent->IsMovable) {
//                                HandleResolvingVectorOnComponent(TransformNodeComponent, BodyNodeComponent, BodyComponentT::GeometricT(1));
//                            } else {
//                                HandleResolvingVectorOnComponent(WithBodyTransform, WithBodyComponent, BodyComponentT::GeometricT(-1));
//                            }
//                            return;
//                        }
//                    }
//                }
//            }
//        };

//        auto RigidBodyComponentSystem = GetSystem<Engine::Components::Physics::RigidBody2DdComponent>();
//
//        std::vector<Engine::PhysicsCalculations::EntityKdTreeNodeContent> ToBeOrdered = {};
//        for (auto& [handle, component] : RigidBodyComponentSystem->components) {
//            if (!component.HasCollisions)
//            {
//                continue;
//            }
//            ToBeOrdered.emplace_back(
//                handle,
//                component.GetEntity().GetComponent<Engine::Components::Transform>(),
//                &component
//            );
//        }
//        Engine::PhysicsCalculations::EntityKdTree KdTree;
//        KdTree.CreateKdTree<2, true>(ToBeOrdered);
//
//        for(auto& [handle, component] : RigidBodyComponentSystem->components)
//        {
//            if (!component.HasCollisions)
//            {
//                continue;
//            }
//
//            Engine::PhysicsCalculations::EntityKdTreeNodeContent ExpectedNodeContent = {
//                handle,
//                component.GetEntity().GetComponent<Engine::Components::Transform>(),
//                &component
//            };
//            auto Node = KdTree.FindNode<2>(ExpectedNodeContent);
//            auto NodeTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
//            const auto MaxSize = component.ComputeMaxDistanceFromTransform();
//
//            auto CollisionsHandler = CollisionsHandlerUtil(
//                RigidBodyComponentSystem,
//                Node
//            );
//            PhysicsCalculations::EntityKdTree::NodeType::ConstNodeTypePtr CurrentNode = Node;
//            auto CurrentTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
//            while (MaxSize < CurrentTransformComponent->Pos.GetVector(NodeTransformComponent->Pos).GetLength())
//            {
//                CurrentNode = const_cast<const PhysicsCalculations::EntityKdTree::NodeType::ConstNodeTypePtr>(Node)->GetParent();
//                CurrentTransformComponent = GetSystem<Engine::Components::Transform>()->GetOf(Node->GetContent().Handle);
//            }
//            CollisionsHandler.ExecuteOnNodes(CurrentNode);
//            // tmp
//            break;
//        }
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