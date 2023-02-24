#pragma once

#include "../../Maths/Shape.hpp"
#include "../../ECS/Component/Component.hpp"
#include "Transform.hpp"
#include "../../Maths/Graph.hpp"
#include "../../Maths/Collisions.hpp"
#include "../Application/EngineApplication.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Engine::Components::Physics
{
    template <typename T>
    static constexpr Maths::Vector2D<T> GetUpVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(1) * -1;
    }
    template <typename T>
    static constexpr Maths::Vector2D<T> GetDownVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(1);
    }

    template <typename T>
    static constexpr Maths::Vector2D<T> GetLeftChildVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(0) * -1;
    }

    template <typename T>
    static constexpr Maths::Vector2D<T> GetRightChildVector2D()
    {
        return Maths::Vector2D<T>::GetUnitVectorOnAxis(0);
    }

    template <size_t Dimensions, typename GeometricT, typename PhysicsT>
    struct IRigidBody
    {
        Maths::Vector2D<PhysicsT> LinearVelocity;
        PhysicsT AngularVelocity;
        Maths::Vector2D<PhysicsT> Force;
        PhysicsT Torque = 0;
        // kg
        PhysicsT Mass = 1;
        PhysicsT MomentOfInertia;
        // 0 - 1
        PhysicsT Bounciness = 0;

        virtual Maths::Point<GeometricT, Dimensions> GetCenterOfMass(
            const Maths::Vector2D<GeometricT>& Scale,
            const GeometricT& RotationDegrees,
            const Maths::Point2D<GeometricT>& OnPos
        ) = 0;
        virtual ~IRigidBody() = default;
    };

    template <typename GeometricT, typename PhysicsT>
    struct IRigidBody2D : public IRigidBody<2, GeometricT, PhysicsT>
    {
        using IBoundingBox2D = Maths::IBoundingBox2D<GeometricT>;
        virtual IBoundingBox2D* GetBoundingBox() = 0;
        virtual void ComputeInertia(const Maths::Vector2D<GeometricT>& Scale) = 0;
        virtual void AddForce(
            const Maths::Vector2D<PhysicsT>& Force,
            const Maths::Point2D<GeometricT>& AtPoint,
            const GeometricT& RotationDegrees,
            const Maths::Vector2D<GeometricT>& Scale,
            const Maths::Point2D<GeometricT>& OnPos
        ) = 0;
    };

    template <typename GeometricT, typename PhysicsT, template <typename ShapeT> typename Shape>
    struct RigidBody2D : IRigidBody2D<GeometricT, PhysicsT>
    {
        Maths::BoundingBox2D<GeometricT, Shape> BoundingBox;

        IRigidBody2D<GeometricT, PhysicsT>::IBoundingBox2D* GetBoundingBox() override
        {
            return static_cast<IRigidBody2D<GeometricT, PhysicsT>::IBoundingBox2D*>(&BoundingBox);
        }
    };

    template <typename GeometricT, typename PhysicsT>
    struct RectangleRigidBody2D : RigidBody2D<GeometricT, PhysicsT, Maths::Rectangle2D>
    {
        Maths::Point2D<GeometricT> GetCenterOfMass(
            const Maths::Vector2D<GeometricT>& Scale,
            const GeometricT& RotationDegrees,
            const Maths::Point2D<GeometricT>& OnPos
        ) override
        {
            return Maths::Point2D<GeometricT>(GeometricT(0), GeometricT(0)) + Maths::Vector2D<GeometricT>{OnPos.Values};
        }

        void ComputeInertia(const Maths::Vector2D<GeometricT>& Scale)
        {
            const auto mass = this->Mass;
            const auto w = this->BoundingBox.Width * Scale.GetX();
            const auto h = this->BoundingBox.Height * Scale.GetY();
            this->MomentOfInertia = mass * (w * w + h * h) / 12;
        }

        void AddForce(
            const Maths::Vector2D<PhysicsT>& Force,
            const Maths::Point2D<GeometricT>& AtPoint,
            const GeometricT& RotationDegrees,
            const Maths::Vector2D<GeometricT>& Scale,
            const Maths::Point2D<GeometricT>& OnPos
        )
        {
            this->Force += Force;
//            const auto Vertices = this->GetBoundingBox()->GetVertices(
//                RotationDegrees,
//                Scale,
//                OnPos
//            );
            const auto CenterOfMass = GetCenterOfMass(
                Scale,
                RotationDegrees,
                OnPos
            );
            auto r = AtPoint.GetVector(CenterOfMass);
            this->Torque += Maths::Vector2D<PhysicsT>{r.Values}.Scalar(this->Force);
//            for (size_t i = 0; i < Vertices.size(); i++)
//            {
//                auto r = Vertices[i].GetVector(CenterOfMass);
//                this->Torque += Maths::Vector2D<PhysicsT>{r.Values}.Scalar(this->Force);
//            }
        }
    };

    using RectangleRigidBody2Dd = RectangleRigidBody2D<double, double>;

    template <typename GT, typename PT, bool Debug = false>
    class RigidBody2DComponent : public Engine::Component
    {
    public:
        using EntityKdTreeNode = Maths::Graph::BinaryTreeNode<EntityHandle>;
        using EntityKdTree = Maths::Graph::KdTree<EntityKdTreeNode>;

        DECLARE_CLASS_TYPE(RigidBody2DComponent, Engine::Component)

        using GeometricT = GT;
        using PhysicsT = PT;
        using PhysicsPoint2DT = Maths::Point2D<PhysicsT>;
        using GeometricPoint2DT = Maths::Point2D<GeometricT>;
        using PhysicsVector2DT = Maths::Vector2D<PhysicsT>;
        using GeometricVector2DT = Maths::Vector2D<GeometricT>;
        using IRigidBody2DT = IRigidBody2D<GeometricT, PhysicsT>;

        bool HasCollisions = true;
        bool HasGravity = true;
        bool IsMovable = true;

        // m/s²
        float Gravity = 9.807f;
        Maths::Vector2D<PhysicsT> GravityDirection = GetDownVector2D<PhysicsT>();

        RigidBody2DComponent()
        {
            if (!GetEntityTransform()) {
                GetEntity().template AddComponent<Engine::Components::Transform>();
            }
        }

        void SetRigidBodies(std::vector<IRigidBody2DT*> Bodies)
        {
            const auto Transform = GetEntityTransform();
            m_RigidBodies = Bodies;
            for (auto & Body : m_RigidBodies)
            {
                ComputeRigidBodyInertia(Transform->Scale, Body);
            }
        }

        template <bool D = Debug, std::enable_if_t<D, bool> = false>
        void OnRender(sf::RenderTarget& RenderTarget) const
        {
            auto EntityTransform = GetEntityTransform();

            for (const auto & Body : m_RigidBodies)
            {
                auto BoundingBox = Body->GetBoundingBox();
                const auto RectangleBoundingBox = dynamic_cast<Maths::RectangleBoundingBox2D<GeometricT>*>(BoundingBox);
                if (RectangleBoundingBox) {
                    sf::RectangleShape Rect;
                    const auto ScaledWidth = RectangleBoundingBox->Width * EntityTransform->Scale.GetX();
                    const auto ScaledHeight = RectangleBoundingBox->Height * EntityTransform->Scale.GetY();
                    Rect.setOrigin(
                        ScaledWidth / 2,
                        ScaledHeight / 2
                    );
                    Rect.setSize({
                         (float) (ScaledWidth),
                         (float) (ScaledHeight)
                     });
                    const auto FinalPos = EntityTransform->Pos + (GeometricVector2DT(
                        RectangleBoundingBox->Position.GetX(),
                        RectangleBoundingBox->Position.GetY()
                    ) * EntityTransform->Scale);
                    Rect.setPosition({
                         (float) (FinalPos.GetX()),
                         (float) (FinalPos.GetY()),
                     });
                    Rect.setRotation(RectangleBoundingBox->Angle + EntityTransform->Angle);
                    Rect.setFillColor(sf::Color::Transparent);
                    Rect.setOutlineColor(sf::Color::White);
                    Rect.setOutlineThickness(-1.f);
                    RenderTarget.draw(Rect);
                }
            }
        }

        void OnUpdate(const float& deltaTime)
        {
            Simulate(deltaTime);
        }

        void Simulate(const float& ElapsedTimeMs, const float& StepMs = 1.0)
        {
            auto currentTimeMs = 0.0;
            auto EntityTransform = GetEntityTransform();
            while (currentTimeMs < ElapsedTimeMs) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(StepMs)));

                for (auto &rigidBody: this->m_RigidBodies) {
                    rigidBody->Force = PhysicsVector2DT{};
                    rigidBody->Torque = 0;

                    AddGravityToBody(
                        EntityTransform->Angle,
                        EntityTransform->Scale,
                        StepMs,
                        rigidBody
                    );
                    ApplyRigidBodyToEntity(
                        StepMs,
                        EntityTransform,
                        rigidBody
                    );

                    // check collisions
                    if (HasCollisions) {
                        auto &EntitiesRegistry = EngineApplication::Get()
                            ->GetScenesLayer()
                            .GetActiveScene()
                            ->GetEntitiesRegistry();
                        auto RigidBodyComponentSystem = EntitiesRegistry
                            .GetSystem<RigidBody2DComponent>();
                        auto TransformSystem = EntitiesRegistry
                            .GetSystem<Engine::Components::Transform>();

                        auto Node = EntitiesRegistry.PhysicsEntityKdTree.FindNode<2, Maths::Point2D<GeometricT>>(
                            this->GetEntity().GetHandle(),
                            [&TransformSystem](EntityHandle handle) -> Maths::Point2D<GeometricT> {
                                return TransformSystem->GetOf(handle)->Pos;
                            }
                        );
                        if (nullptr != Node) {
                            auto NodeTransformComponent = TransformSystem->GetOf(Node->GetContent());
                            const auto MaxSize = ComputeMaxDistanceFromTransform();

                            const EntityKdTreeNode *CurrentNode = Node;
                            auto CurrentTransformComponent = TransformSystem->GetOf(Node->GetContent());
                            while (MaxSize <
                                   CurrentTransformComponent->Pos.GetVector(NodeTransformComponent->Pos).GetLength()) {
                                CurrentNode = static_cast<const EntityKdTreeNode *>(Node)->GetParent();
                                CurrentTransformComponent = TransformSystem->GetOf(Node->GetContent());
                            }

                            rigidBody->Force = PhysicsVector2DT{};
                            rigidBody->Torque = 0;

                            ExecuteOnNodes(
                                RigidBodyComponentSystem,
                                Node,
                                NodeTransformComponent,
                                CurrentNode
                            );
                            ApplyRigidBodyToEntity(
                                StepMs,
                                EntityTransform,
                                rigidBody
                            );
                        }
                    }
                    // tmp because would not work with multiple rigidbodies?
                    break;
                }

                currentTimeMs += StepMs;
            }
        }

        GeometricT ComputeMaxDistanceFromTransform() const
        {
            GeometricT MaxDistance = 0;
            for (const auto& Body : this->m_RigidBodies)
            {
                const auto& BoundingBox = Body->GetBoundingBox();
                const auto BodyMaxDistance = BoundingBox->ComputeMaxDistanceFromPosition() + GeometricVector2DT(
                    BoundingBox->Position.GetX(),
                    BoundingBox->Position.GetY()
                ).GetLength();
                if (MaxDistance < BodyMaxDistance)
                {
                    MaxDistance = BodyMaxDistance;
                }
            }
            return MaxDistance;
        }

        // @deprecated
        bool IsPointInside(const Maths::Point2D<GeometricT>& Point) const
        {
            IRigidBody2DT*& IgnoredInRigidBody = nullptr;
            return IsPointInside(Point, IgnoredInRigidBody);
        }

        // @deprecated
        bool IsPointInside(const Maths::Point2D<GeometricT>& Point, IRigidBody2DT*& InRigidBody) const
        {
            for (const auto& Body : this->m_RigidBodies)
            {
                const auto& BoundingBox = Body->GetBoundingBox();
                auto EntityTransform = GetEntityTransform();
                const auto FinalPoint = BoundingBox->Position
                    - GeometricVector2DT(EntityTransform->Pos.x, EntityTransform->Pos.y)
                ;
                if (BoundingBox->IsPointInside(FinalPoint))
                {
                    InRigidBody = Body;
                    return true;
                }
            }
            return false;
        }

        std::vector<IRigidBody2DT*>& GetRigidBodies()
        {
            return m_RigidBodies;
        }
    private:
        // @todo can be useful to have multiple rigidbodies but will not work with this implementation bc of gravity?
        std::vector<IRigidBody2DT*> m_RigidBodies;

        static void ComputeRigidBodyInertia(
            const Maths::Vector2D<GeometricT>& Scale,
            IRigidBody2DT* RigidBody
        )
        {
            RigidBody->ComputeInertia(Scale);
        }

        void AddGravityToBody(
            const GeometricT& RotationDegrees,
            const Maths::Vector2D<GeometricT>& Scale,
            const float& StepMs,
            IRigidBody2DT* RigidBody
        )
        {
            const auto GravityVec = GravityDirection * Gravity * (PhysicsT(HasGravity ? 1 : 0)) * (StepMs / 1000);
            const Maths::Point2D<GeometricT> OnPos = Maths::Point2D<GeometricT>(
                GeometricT(0),
                GeometricT(0)
            );
            const Maths::Point2D<GeometricT> AtPoint = RigidBody->GetCenterOfMass(
                Scale,
                RotationDegrees,
                OnPos
            );
            RigidBody->AddForce(
                GravityVec,
                AtPoint,
                RotationDegrees,
                Scale,
                OnPos
            );
        }

        void ApplyRigidBodyToEntity(
            const float& StepMs,
            Engine::Components::Transform* EntityTransform,
            IRigidBody2DT* RigidBody
        )
        {
            const PhysicsVector2DT linearAcceleration = PhysicsVector2DT(
                RigidBody->Force.GetX() / RigidBody->Mass,
                RigidBody->Force.GetY() / RigidBody->Mass
            );
            RigidBody->LinearVelocity.GetX() += linearAcceleration.GetX() * StepMs;
            RigidBody->LinearVelocity.GetY() += linearAcceleration.GetY() * StepMs;
            const auto angularAcceleration = RigidBody->Torque / RigidBody->MomentOfInertia;
            EntityTransform->Angle += RigidBody->AngularVelocity * StepMs;
            RigidBody->AngularVelocity += angularAcceleration * StepMs;

            EntityTransform->Pos.GetX() += RigidBody->LinearVelocity.GetX() * StepMs;
            EntityTransform->Pos.GetY() += RigidBody->LinearVelocity.GetY() * StepMs;
        }

        [[nodiscard]] Engine::Components::Transform* GetEntityTransform() const
        {
            return GetEntity().template GetComponent<Engine::Components::Transform>();
        }

        void ExecuteOnNodes(
            ComponentSystem<RigidBody2DComponent>* ComponentSystem,
            EntityKdTreeNode* NodeUnderCheck,
            Engine::Components::Transform* NodeTransform,
            const EntityKdTreeNode* FromNode
        ) const
        {
            if (nullptr != FromNode->GetLeftChild()) {
                CheckAndHandleCollision(
                    ComponentSystem,
                    NodeUnderCheck,
                    NodeTransform,
                    FromNode->GetLeftChild()->GetContent()
                );
                ExecuteOnNodes(
                    ComponentSystem,
                    NodeUnderCheck,
                    NodeTransform,
                    FromNode->GetLeftChild()
                );
            }
            if (nullptr != FromNode->GetRightChild()) {
                CheckAndHandleCollision(
                    ComponentSystem,
                    NodeUnderCheck,
                    NodeTransform,
                    FromNode->GetRightChild()->GetContent()
                );
                ExecuteOnNodes(
                    ComponentSystem,
                    NodeUnderCheck,
                    NodeTransform,
                    FromNode->GetRightChild()
                );
            }
        }

        void CheckAndHandleCollision(
            ComponentSystem<RigidBody2DComponent>* ComponentSystem,
            EntityKdTreeNode* NodeUnderCheck,
            Engine::Components::Transform* NodeTransform,
            EntityHandle WithHandle
        ) const
        {
            if (WithHandle == NodeUnderCheck->GetContent())
                return;

            auto WithBodyComponent = ComponentSystem->GetOf(WithHandle);
            auto WithBodyTransform = WithBodyComponent->GetEntity().template GetComponent<Engine::Components::Transform>();
            for (const auto& NodeBody : this->m_RigidBodies)
            {
                for (const auto& WithBody : WithBodyComponent->m_RigidBodies)
                {
                    std::vector<GeometricVector2DT> Axes = {};
                    std::vector<GeometricPoint2DT> NodeProjectablePoints = {};
                    std::vector<GeometricPoint2DT> ProjectablePoints = {};
                    Maths::Collisions::SAT::ComputeAxesAndPointsToCheck<GeometricT, 2>(
                        NodeTransform->Scale,
                        WithBodyTransform->Scale,
                        NodeTransform->Angle,
                        WithBodyTransform->Angle,
                        NodeTransform->Pos,
                        WithBodyTransform->Pos,
                        NodeBody->GetBoundingBox(),
                        WithBody->GetBoundingBox(),
                        Axes,
                        NodeProjectablePoints,
                        ProjectablePoints
                    );

                    GeometricPoint2DT CollisionPoint = GeometricPoint2DT(GeometricT(0), GeometricT(0));
                    GeometricVector2DT CollisionNormal;
                    GeometricT OverlapSizeResult = GeometricT(-1);
                    if (Maths::Collisions::SAT::CheckOverlap(
                        Axes,
                        NodeProjectablePoints,
                        ProjectablePoints,
                        CollisionNormal,
                        OverlapSizeResult,
                        CollisionPoint
                    )) {
                        const auto OverlapResolvingVector = CollisionNormal * OverlapSizeResult;
                        const auto Bounciness = (NodeBody->Bounciness + WithBody->Bounciness) / 2;
                        if (this->IsMovable) {
                            NodeTransform->Pos = NodeTransform->Pos + (OverlapResolvingVector);
                            const PhysicsT VelocityNormalScalar = NodeBody->LinearVelocity.Scalar(CollisionNormal);
                            const auto ForceToApply = CollisionNormal * (-(1 + Bounciness)
                                * VelocityNormalScalar
                                / CollisionNormal.GetSquareLength());
                            NodeBody->LinearVelocity = PhysicsVector2DT{};
                            // tmp
                            NodeBody->AddForce(
                                ForceToApply,
                                CollisionPoint,
                                NodeTransform->Angle,
                                NodeTransform->Scale,
                                NodeTransform->Pos
                            );
                        } else {
                            WithBodyTransform->Pos = WithBodyTransform->Pos + (OverlapResolvingVector * -1);
                            const PhysicsT VelocityNormalScalar = WithBody->LinearVelocity.Scalar(CollisionNormal);
                            const auto ForceToApply = CollisionNormal * (-(1 + Bounciness)
                                  * VelocityNormalScalar
                                  / CollisionNormal.GetSquareLength());
                            WithBody->LinearVelocity = PhysicsVector2DT{};
                            // tmp
                            WithBody->AddForce(
                                ForceToApply,
                                CollisionPoint,
                                WithBodyTransform->Angle,
                                WithBodyTransform->Scale,
                                WithBodyTransform->Pos
                            );
                        }
                        return;
                    }
                }
            }
        }
    };

    using RigidBody2DdComponent = RigidBody2DComponent<double, double, true>;
}