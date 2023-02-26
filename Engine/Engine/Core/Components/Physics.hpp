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
    // All distances are in centimeters
    // All forces are in m/s²
    // All masses are in kg

    template <typename GeometricType, size_t Dimensions>
    Maths::Collisions::CollisionManifold<GeometricType, Dimensions> CollideBoundingBoxes(
        const Maths::Vector<GeometricType, Dimensions>& ScaleA,
        const Maths::Vector<GeometricType, Dimensions>& ScaleB,
        const GeometricType& RotationDegreesA,
        const GeometricType& RotationDegreesB,
        const Maths::Point2D<GeometricType>& OnPosA,
        const Maths::Point2D<GeometricType>& OnPosB,
        const Maths::IBoundingBox2D<GeometricType>* ShapeA,
        const Maths::IBoundingBox2D<GeometricType>* ShapeB
    )
    {
        auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
        auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
        if (CastedShapeA && CastedShapeB) {
            return Maths::Collisions::SAT::Collide(
                ScaleA,
                ScaleB,
                RotationDegreesA,
                RotationDegreesB,
                OnPosA,
                OnPosB,
                static_cast<const Maths::IShape*>(CastedShapeA),
                static_cast<const Maths::IShape*>(CastedShapeB)
            );
        }

        throw std::runtime_error("No matching function found.");
    }

    template <typename GeometricType, size_t Dimensions>
    bool AreBoundingBoxLikelyToCollide(
        const Maths::Vector<GeometricType, Dimensions>& ScaleA,
        const Maths::Vector<GeometricType, Dimensions>& ScaleB,
        const GeometricType& RotationDegreesA,
        const GeometricType& RotationDegreesB,
        const Maths::Point2D<GeometricType>& OnPosA,
        const Maths::Point2D<GeometricType>& OnPosB,
        const Maths::IBoundingBox2D<GeometricType>* ShapeA,
        const Maths::IBoundingBox2D<GeometricType>* ShapeB
    )
    {
        auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
        auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
        if (CastedShapeA && CastedShapeB) {
            return Maths::Collisions::SAP::LikelyToCollide(
                ScaleA,
                ScaleB,
                RotationDegreesA,
                RotationDegreesB,
                OnPosA,
                OnPosB,
                static_cast<const Maths::IShape*>(CastedShapeA),
                static_cast<const Maths::IShape*>(CastedShapeB)
            );
        }

        throw std::runtime_error("No matching function found.");
    }

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
        PhysicsT Torque = 0.f;
        // kg
        PhysicsT Mass = 1.f;
        // m4
        PhysicsT MomentOfInertiaForZ;

        // @todo maybe move this to PhysicsMaterial
        // 0 - 1, restitution
        PhysicsT Restitution = 0;
        // @todo use threshold to stop calculations?
        PhysicsT BouncinessThreshold = 0.5f;
        // 0 - 1, friction on surface
        PhysicsT StaticFriction = 0.25f;
        PhysicsT DynamicFriction = 0.25f;

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
            // meters
            const auto w = this->BoundingBox.Width * Scale.GetX() / 100.f;
            const auto h = this->BoundingBox.Height * Scale.GetY() / 100.f;
            this->MomentOfInertiaForZ = mass * (w * w + h * h) / 12.f;
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
            const auto CenterOfMass = GetCenterOfMass(
                Scale,
                RotationDegrees,
                OnPos
            );
            // in meters
            auto r = AtPoint.GetVectorTo(CenterOfMass) / 100;
            this->Torque += r ^ this->Force;
        }
    };

    using RectangleRigidBody2Dd = RectangleRigidBody2D<double, double>;

    template <typename GT, typename PT, bool DebugShapes = false, bool DebugPoints = false>
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

        template <typename GeometricT, size_t Dimensions, typename RigidBodyT>
        struct CollisionManifoldWithBodies : public Maths::Collisions::CollisionManifold<GeometricT, Dimensions>
        {
            RigidBodyT* FirstRigidBody = nullptr;
            RigidBodyT* SecondRigidBody = nullptr;
            RigidBody2DComponent* SecondBodyComponent = nullptr;
            GeometricPoint2DT FirstCenterOfMassAtCollision;
            GeometricPoint2DT SecondCenterOfMassAtCollision;
        };

        static constexpr PT Gravity = PT(9.80665f);
        static PhysicsVector2DT GravityDirection;
        static constexpr PT LinearDrag = PT(0.05f);
        static constexpr PT AngularDrag = PT(0.5f);
        // 1 mm
        static constexpr GT DistanceStep = PT(0.001f);

        float GravityScale = 1.f;
        bool HasCollisions = true;
        bool IsStatic = false;


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

        template <bool DShapes = DebugShapes, bool DPoints = DebugPoints, std::enable_if_t<(DShapes || DPoints), bool> = false>
        void OnRender(sf::RenderTarget& RenderTarget) const
        {
            auto ActiveCamera = EngineApplication::Get()
                ->GetScenesLayer()
                .GetActiveScene()
                ->GetActiveCamera();
            auto PixelSize = 1.f;
            if (dynamic_cast<Engine::Camera::Camera2D<GeometricT>*>(ActiveCamera))
            {
                PixelSize = PixelSize / static_cast<Engine::Camera::Camera2D<GeometricT>*>(ActiveCamera)->PixelCentimeterRatio;
            }

            auto EntityTransform = GetEntityTransform();

            for (const auto & Body : m_RigidBodies)
            {
                auto BoundingBox = Body->GetBoundingBox();
                const auto RectangleBoundingBox = dynamic_cast<Maths::RectangleBoundingBox2D<GeometricT>*>(BoundingBox);
                if (RectangleBoundingBox) {
                    if constexpr (DShapes)
                    {
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
                        Rect.setOutlineThickness(PixelSize);
                        RenderTarget.draw(Rect);
                    }

                    if constexpr (DPoints)
                    {
                        // Draw vertices manually to verify previous drawing
                        for (const auto& Vertice : BoundingBox->GetVertices(
                            EntityTransform->Angle,
                            EntityTransform->Scale,
                            EntityTransform->Pos
                        ))
                        {
                            sf::CircleShape Point;
                            // 2 mm
                            Point.setRadius(PixelSize * 2);
                            Point.setOrigin(PixelSize * 2, PixelSize * 2);
                            Point.setFillColor(sf::Color::Red);
                            Point.setPosition({
                                (float) Vertice.GetX(),
                                (float) Vertice.GetY()
                            });
                            RenderTarget.draw(Point);
                            sf::Text PositionText;
                            PositionText.setFillColor(sf::Color::Red);
                            std::ostringstream PosString;
                            PosString << std::round(Vertice.GetX()) << " , " << std::round(Vertice.GetY());
                            PositionText.setString(PosString.str());
                            sf::Font Lato;
                            Lato.loadFromFile("../../Assets/Lato/Lato-Regular.ttf");
                            PositionText.setFont(Lato);
                            PositionText.setCharacterSize(12);
                            PositionText.setPosition({
                                 (float) Vertice.GetX(),
                                 (float) Vertice.GetY() - (PixelSize * 10.f)
                             });
                            PositionText.setOrigin(
                                PositionText.getGlobalBounds().width / 2,
                                PositionText.getGlobalBounds().height / 2
                            );
                            PositionText.setScale({PixelSize, PixelSize});
                            RenderTarget.draw(PositionText);
                        }
                    }
                }
            }
        }

        void OnUpdate(const float& deltaTime)
        {
            Simulate(deltaTime);
        }

        void Simulate(const float& ElapsedTimeMs, const float& StepMs = 1.f)
        {
            if (this->IsStatic) {
                return;
            }
            auto currentTimeMs = 0.0;
            auto EntityTransform = GetEntityTransform();
            while (currentTimeMs < ElapsedTimeMs) {
                for (auto &rigidBody: this->m_RigidBodies) {
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

                        auto TransformComponent = GetEntityTransform();
                        const auto MaxSize = ComputeMaxDistanceFromTransform();

                        std::vector<RigidBody2DComponent*> LikelyToCollide;
                        for (auto& [handle, WithComponent] : RigidBodyComponentSystem->components)
                        {
                            if (&WithComponent == this)
                                continue;

                            auto WithBodyTransform = WithComponent.GetEntity().template GetComponent<Engine::Components::Transform>();
                            for (auto& WithBodyComponent : WithComponent.m_RigidBodies)
                            {
                                if (AreBoundingBoxLikelyToCollide<GeometricT, 2>(
                                    TransformComponent->Scale,
                                    WithBodyTransform->Scale,
                                    TransformComponent->Angle,
                                    WithBodyTransform->Angle,
                                    TransformComponent->Pos,
                                    WithBodyTransform->Pos,
                                    rigidBody->GetBoundingBox(),
                                    WithBodyComponent->GetBoundingBox()
                                ))
                                {
                                    LikelyToCollide.push_back(&WithComponent);
                                }
                            }
                        }

                        std::vector<CollisionManifoldWithBodies<GeometricT, 2, IRigidBody2DT>> ContactManifolds;
                        for (auto& CollidingBody : LikelyToCollide)
                        {
                            CheckAndHandleCollision(
                                RigidBodyComponentSystem,
                                rigidBody,
                                TransformComponent,
                                CollidingBody,
                                ContactManifolds
                            );
                        }

                        for (const auto& Manifold : ContactManifolds)
                        {
                            ApplyForceFromCollision(StepMs, Manifold);
                        }
                    }
                    // tmp, see comment on array of rigidbodies
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
        // @todo can be useful to have multiple rigidbodies but will not work with this implementation?
        std::vector<IRigidBody2DT*> m_RigidBodies;

        static void ComputeRigidBodyInertia(
            const Maths::Vector2D<GeometricT>& Scale,
            IRigidBody2DT* RigidBody
        )
        {
            RigidBody->ComputeInertia(Scale);
        }

        void ApplyForceFromCollision(
            const float& StepMs,
            CollisionManifoldWithBodies<GeometricT, 2, IRigidBody2DT> CollisionManifold
        )
        {
            const auto TransformComponent = GetEntityTransform();
            auto OnBody = CollisionManifold.FirstRigidBody;
            auto WithBody = CollisionManifold.SecondRigidBody;
            auto WithBodyComponent = CollisionManifold.SecondBodyComponent;
            if (this->IsStatic && WithBodyComponent->IsStatic)
            {
                return;
            }
            const auto WithBodyTransform = WithBodyComponent->GetEntityTransform();
            // direction from A to B
            const auto Normal = CollisionManifold.Normal * -1;

            auto RelativeVelocity = WithBody->LinearVelocity - OnBody->LinearVelocity;
            auto RelativeVelocityOnNormal = RelativeVelocity.Scalar(Normal);
            if (RelativeVelocityOnNormal > PhysicsT(0))
            {
                RelativeVelocityOnNormal = 0;
            }

            const auto ContactsCount = CollisionManifold.Contacts.size();

            // @todo move this in function
            const auto Restitution = std::min(OnBody->Restitution, WithBody->Restitution);

            // (-(1 + e)((Va - Vb) ⋅ n)) / (1 / ma + 1 / mb)
            const auto jN = (-(PhysicsT(1) + Restitution) * RelativeVelocityOnNormal) / (1 / OnBody->Mass + 1 / WithBody->Mass);

            // @todo move this in function
            const auto StaticFrictionCoef = std::clamp((OnBody->StaticFriction + WithBody->StaticFriction) / 2, PhysicsT(0), PhysicsT(1));
            // @todo move this in function
            const auto DynamicFrictionCoef = std::clamp((OnBody->DynamicFriction + WithBody->DynamicFriction) / 2, PhysicsT(0), PhysicsT(1));

            const auto ComputeFrictionImpulse = [&Normal, &jN, &StaticFrictionCoef, &DynamicFrictionCoef, &OnBody, &WithBody]()->PhysicsVector2DT
            {
                const auto RelativeVelocity = (WithBody->LinearVelocity - OnBody->LinearVelocity);
                // Tangent must be in similar direction than velocity
                const auto TangentialVector = (RelativeVelocity - (Normal * RelativeVelocity.Scalar(Normal))).GetNormalized();
                const auto RelativeVelocityOnTangent = Maths::ProjectOnUnitVector(
                    RelativeVelocity,
                    TangentialVector * -1
                );
                // (-(1 + e)((Va - Vb) ⋅ t)) / (1 / ma + 1 / mb)
                auto jT = RelativeVelocityOnTangent / (1 / OnBody->Mass + 1 / WithBody->Mass);

                // Coulomb's Law
                if (std::abs(jT) < jN * StaticFrictionCoef) {
                    // static friction
                    return TangentialVector * jT;
                }
                // dynamic friction
                return TangentialVector * -jN * DynamicFrictionCoef;
            };

            const auto NormalImpulse = Normal * jN;

            GeometricPoint2DT OnPos = GeometricPoint2DT(GeometricT(0), GeometricT(0));
            if (this->IsStatic)
            {
                WithBody->LinearVelocity = WithBody->LinearVelocity + NormalImpulse * (1 / WithBody->Mass);
                const auto FrictionImpulse = ComputeFrictionImpulse();
                WithBody->LinearVelocity = WithBody->LinearVelocity + FrictionImpulse * (1 / WithBody->Mass);

                PhysicsT TotalAngularImpulse = PhysicsT(0);
                for (const auto& ContactPoint : CollisionManifold.Contacts)
                {
                    const auto VecFromCenterOfMass = (ContactPoint).GetVectorTo(CollisionManifold.SecondCenterOfMassAtCollision);
                    auto AngularImpulse = (
                        ((jN * 1 / WithBody->Mass) / ContactsCount)
                        * (1 / WithBody->MomentOfInertiaForZ)
                        // meters
                        * ((VecFromCenterOfMass / 100.f) ^ (Normal))
                    );
                    TotalAngularImpulse += AngularImpulse;
                }
                WithBody->AngularVelocity = WithBody->AngularVelocity + TotalAngularImpulse;
                return;
            }
            if (WithBodyComponent->IsStatic)
            {
                OnBody->LinearVelocity = OnBody->LinearVelocity - NormalImpulse * (1 / OnBody->Mass);
                const auto FrictionImpulse = ComputeFrictionImpulse();
                OnBody->LinearVelocity = OnBody->LinearVelocity - FrictionImpulse * (1 / OnBody->Mass);

                PhysicsT TotalAngularImpulse = PhysicsT(0);
                for (const auto& ContactPoint : CollisionManifold.Contacts)
                {
                    const auto VecFromCenterOfMass = ContactPoint.GetVectorTo(CollisionManifold.FirstCenterOfMassAtCollision);
                    auto AngularImpulse = (
                        ((jN * 1 / OnBody->Mass) / ContactsCount)
                        * (1 / OnBody->MomentOfInertiaForZ)
                        * ((VecFromCenterOfMass / 100.f) ^ (Normal))
                    );
                    TotalAngularImpulse += AngularImpulse;
                }
                OnBody->AngularVelocity = OnBody->AngularVelocity - TotalAngularImpulse;
                return;
            }

            // apply on both bodies
            OnBody->LinearVelocity = OnBody->LinearVelocity - NormalImpulse * (1 / OnBody->Mass);
            WithBody->LinearVelocity = WithBody->LinearVelocity + NormalImpulse * (1 / WithBody->Mass);
            const auto FrictionImpulse = ComputeFrictionImpulse();
            OnBody->LinearVelocity = OnBody->LinearVelocity - FrictionImpulse * (1 / OnBody->Mass);
            WithBody->LinearVelocity = WithBody->LinearVelocity + FrictionImpulse * (1 / WithBody->Mass);

            PhysicsT TotalAngularImpulseA = PhysicsT(0);
            PhysicsT TotalAngularImpulseB = PhysicsT(0);

            for (const auto& ContactPoint : CollisionManifold.Contacts)
            {
                {
                    const auto VecFromCenterOfMass = ContactPoint.GetVectorTo(CollisionManifold.FirstCenterOfMassAtCollision);
                    auto AngularImpulse = (
                        ((jN * 1 / OnBody->Mass) / ContactsCount)
                        * (1 / OnBody->MomentOfInertiaForZ)
                        * ((VecFromCenterOfMass / 100.f) ^ (Normal))
                    );
                    TotalAngularImpulseA += AngularImpulse;
                }
                {
                    const auto VecFromCenterOfMass = ContactPoint.GetVectorTo(CollisionManifold.SecondCenterOfMassAtCollision);
                    auto AngularImpulse = (
                        ((jN * 1 / WithBody->Mass) / ContactsCount)
                        * (1 / WithBody->MomentOfInertiaForZ)
                        * ((VecFromCenterOfMass / 100.f) ^ (Normal))
                    );
                    TotalAngularImpulseB += AngularImpulse;
                }
            }
            OnBody->AngularVelocity = OnBody->AngularVelocity - TotalAngularImpulseA;
            WithBody->AngularVelocity = WithBody->AngularVelocity + TotalAngularImpulseB;
        }

        void ApplyRigidBodyToEntity(
            const float& StepMs,
            Engine::Components::Transform* EntityTransform,
            IRigidBody2DT* RigidBody
        )
        {
            if (this->IsStatic) {
                return;
            }
            const auto GravityVec = GravityDirection * GravityScale * Gravity;
            const PhysicsVector2DT linearAcceleration = PhysicsVector2DT(
                RigidBody->Force.GetX() / RigidBody->Mass,
                RigidBody->Force.GetY() / RigidBody->Mass
            );
            const auto StepMsMultiplier = (StepMs / 1000);
            RigidBody->LinearVelocity.GetX() += (linearAcceleration.GetX() + GravityVec.GetX()) * StepMsMultiplier;
            RigidBody->LinearVelocity.GetY() += (linearAcceleration.GetY() + GravityVec.GetY()) * StepMsMultiplier;
            const auto angularAcceleration = RigidBody->Torque / RigidBody->MomentOfInertiaForZ;
            EntityTransform->Angle += RigidBody->AngularVelocity * StepMsMultiplier;
            if (EntityTransform->Angle >= 360.f)
            {
                EntityTransform->Angle -= 360.f;
            }
            RigidBody->AngularVelocity += angularAcceleration * StepMsMultiplier;

            // in centimeters
            EntityTransform->Pos.GetX() += (RigidBody->LinearVelocity.GetX() * 100.f) * StepMsMultiplier;
            EntityTransform->Pos.GetY() += (RigidBody->LinearVelocity.GetY() * 100.f) * StepMsMultiplier;

            // reset applied force each time it is applied
            RigidBody->Force = PhysicsVector2DT{};
            RigidBody->Torque = 0;

            // apply drag to simulate air friction
            RigidBody->LinearVelocity = RigidBody->LinearVelocity * std::clamp(PhysicsT(1.f) - LinearDrag * StepMsMultiplier, PhysicsT(0.f), PhysicsT(1.f));
            RigidBody->AngularVelocity *= std::clamp(PhysicsT(1.f) - AngularDrag * StepMsMultiplier, PhysicsT(0.f), PhysicsT(1.f));
        }

        [[nodiscard]] Engine::Components::Transform* GetEntityTransform() const
        {
            return GetEntity().template GetComponent<Engine::Components::Transform>();
        }

        void CheckAndHandleCollision(
            ComponentSystem<RigidBody2DComponent>* ComponentSystem,
            IRigidBody2DT* BodyToCheck,
            Engine::Components::Transform* TransformComponent,
            RigidBody2DComponent* WithBodyComponent,
            std::vector<CollisionManifoldWithBodies<GeometricT, 2, IRigidBody2DT>>& CollisionManifolds
        ) const
        {
            auto WithBodyTransform = WithBodyComponent->GetEntity().template GetComponent<Engine::Components::Transform>();
            if (this->IsStatic && WithBodyComponent->IsStatic)
            {
                return;
            }
            for (const auto& WithBody : WithBodyComponent->m_RigidBodies)
            {
                auto CollisionManifold = CollideBoundingBoxes<GeometricT, 2>(
                    TransformComponent->Scale,
                    WithBodyTransform->Scale,
                    TransformComponent->Angle,
                    WithBodyTransform->Angle,
                    TransformComponent->Pos,
                    WithBodyTransform->Pos,
                    BodyToCheck->GetBoundingBox(),
                    WithBody->GetBoundingBox()
                );
                if (CollisionManifold.HasCollided()) {
                    CollisionManifoldWithBodies<GeometricT, 2, IRigidBody2DT> Manifold;
                    Manifold.Normal = CollisionManifold.Normal;
                    Manifold.Contacts = CollisionManifold.Contacts;
                    Manifold.Depth = CollisionManifold.Depth;
                    Manifold.FirstRigidBody = BodyToCheck;
                    Manifold.SecondRigidBody = WithBody;
                    Manifold.SecondBodyComponent = WithBodyComponent;
                    Manifold.FirstCenterOfMassAtCollision = BodyToCheck->GetCenterOfMass(
                        TransformComponent->Scale,
                        TransformComponent->Angle,
                        TransformComponent->Pos
                    );
                    Manifold.SecondCenterOfMassAtCollision = WithBody->GetCenterOfMass(
                        WithBodyTransform->Scale,
                        WithBodyTransform->Angle,
                        WithBodyTransform->Pos
                    );
                    CollisionManifolds.push_back(Manifold);

                    const auto OverlapResolvingVector = CollisionManifold.Normal * std::max(CollisionManifold.Depth, DistanceStep);
                    if (this->IsStatic) {
                        WithBodyTransform->Pos = WithBodyTransform->Pos + (OverlapResolvingVector * -1);
                    } else if (WithBodyComponent->IsStatic) {
                        TransformComponent->Pos = TransformComponent->Pos + OverlapResolvingVector;
                    } else {
                        const auto HalfResolve = OverlapResolvingVector * (1 / 2);
                        TransformComponent->Pos = TransformComponent->Pos + HalfResolve;
                        WithBodyTransform->Pos = WithBodyTransform->Pos + (HalfResolve * -1);
                    }
                }

                // tmp, see comment on array of rigidbodies
                break;
            }
        }
    };

    template <typename GT, typename PT, bool DebugShapes, bool DebugPoints>
    RigidBody2DComponent<GT, PT, DebugShapes, DebugPoints>::PhysicsVector2DT RigidBody2DComponent<GT, PT, DebugShapes, DebugPoints>::GravityDirection = GetDownVector2D<PhysicsT>();

    using RigidBody2DdComponent = RigidBody2DComponent<double, double, true, true>;
}