#pragma once

#include <cstddef>

#include "../Components/Physics.hpp"

namespace Engine::Physics {
    class IPhysicsSimulator
    {
    public:
        virtual void Simulate(
            const float& DeltaTime,
            Engine::EntitiesRegistry* EntitiesRegistry
        ) = 0;
        virtual RTTI::ClassType* GetBodyType() = 0;
        virtual ~IPhysicsSimulator() = default;
    };

    template <typename RigidBodyComponentT>
    class Physics2DSimulator : public IPhysicsSimulator {
    private:
        // 1 mm
        static constexpr RigidBodyComponentT::GeometricT DistanceStep = typename RigidBodyComponentT::GeometricT(0.1f);

        using CollisionManifoldT = Maths::Collisions::CollisionManifold<
            typename RigidBodyComponentT::GeometricT,
            RigidBodyComponentT::Dimensions
        >;

        struct CollisionManifoldWithBodies : public CollisionManifoldT
        {
            RigidBodyComponentT* FirstRigidBody = nullptr;
            RigidBodyComponentT* SecondRigidBody = nullptr;
            RigidBodyComponentT::GeometricPointT FirstCenterOfMassAtCollision;
            RigidBodyComponentT::GeometricPointT SecondCenterOfMassAtCollision;
        };

        struct CachedEntity {
            RigidBodyComponentT* Body = nullptr;
            Engine::Components::Transform* Transform = nullptr;
        };
    public:
        void Simulate(
            const float& DeltaTime,
            Engine::EntitiesRegistry* EntitiesRegistry
        ) override
        {
            std::vector<CachedEntity> Bodies;
            for (auto& [Handle, Component] : EntitiesRegistry->GetSystem<RigidBodyComponentT>()->components)
            {
                auto Transform = Component.GetEntityTransform();

                Bodies.push_back({
                    &Component,
                    Transform
                });
            }

            // choose axis Y because with gravity it should be the most reliable for performance
            const auto Axis = RigidBodyComponentT::GeometricVectorT::GetUnitVectorOnAxis(1);

            const auto StepMs = 2.f;
            Timestep Elapsed = 0;
            while (Elapsed < DeltaTime) {
                SimulateRigidBodies(StepMs, Bodies);
                auto SortComp = [&Axis](const CachedEntity& A, const CachedEntity& B)->bool {
                    auto MinA = A.Body->GetRigidBody()->GetMinProjectedOnAxis(Axis);
                    auto MinB = B.Body->GetRigidBody()->GetMinProjectedOnAxis(Axis);
                    return MinA < MinB;
                };
                if (!std::is_sorted(Bodies.begin(), Bodies.end(), SortComp)) {
                    std::sort(Bodies.begin(), Bodies.end(), SortComp);
                }

                HandleCollisionsOnBodies(Axis, Bodies);
                Elapsed += StepMs;
            }
            // handle left time
            const auto Left = DeltaTime - Elapsed;
            if (Left > 0) {
                SimulateRigidBodies(Left, Bodies);

                HandleCollisionsOnBodies(Axis, Bodies);
            }
        }

        RTTI::ClassType* GetBodyType() override
        {
            return RigidBodyComponentT::getClassType();
        }
    private:
        void SimulateRigidBodies(
            const float& DeltaTime,
            std::vector<CachedEntity>& SortedBodiesOnAxis
        ) const
        {
            for (auto& CachedEntity : SortedBodiesOnAxis) {
                CachedEntity.Body->SimulateRigidBody(DeltaTime, CachedEntity.Transform);
                CachedEntity.Body->HandleRecomputeCachedProperties();
            }
        }

        void HandleCollisionsOnBodies(
            const RigidBodyComponentT::GeometricVectorT& Axis,
            std::vector<CachedEntity>& SortedBodiesOnAxis
        ) const
        {
            std::vector<std::array<CachedEntity, 2>> LikelyToCollide;
            LikelyToCollide.reserve(SortedBodiesOnAxis.size() / 2);
            std::list<CachedEntity> ActiveIntervals;
            for (auto& CachedEntity : SortedBodiesOnAxis)
            {
                auto Body = CachedEntity.Body;
                auto BodyTransform = CachedEntity.Transform;

                for (auto CachedEntityIt = ActiveIntervals.rbegin(); CachedEntityIt != ActiveIntervals.rend(); CachedEntityIt++) {
                    auto WithCachedEntity = *CachedEntityIt;
                    auto WithCachedEntityBody = WithCachedEntity.Body;
                    if (Body->IsStatic && WithCachedEntityBody->IsStatic)
                    {
                        continue;
                    }

                    if (AreCachedEntitiesLikelyToCollide<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                        Axis,
                        CachedEntity,
                        WithCachedEntity
                    )) {
                        LikelyToCollide.push_back({CachedEntity, WithCachedEntity});
                    } else {
                        // we do not need to keep it as we are only going further
                        ActiveIntervals.erase(std::next(CachedEntityIt).base());
                    }
                }
                ActiveIntervals.push_back(CachedEntity);
            }

            std::vector<CollisionManifoldWithBodies> ContactManifolds;
            ContactManifolds.reserve(LikelyToCollide.size() / 2);
            for (auto& [CollidingBodyA, CollidingBodyB] : LikelyToCollide)
            {
                CheckAndHandleCollision(
                    CollidingBodyA,
                    CollidingBodyB,
                    ContactManifolds
                );
            }

            for (const auto& Manifold : ContactManifolds)
            {
                ApplyForceFromCollision(Manifold);
            }
        }

        void CheckAndHandleCollision(
            CachedEntity& CachedEntityA,
            CachedEntity& CachedEntityB,
            std::vector<CollisionManifoldWithBodies>& CollisionManifolds
        ) const
        {
            auto BodyAComponent = CachedEntityA.Body;
            auto BodyBComponent = CachedEntityB.Body;
            BodyAComponent->HandleRecomputeCachedProperties();
            BodyBComponent->HandleRecomputeCachedProperties();
            auto BodyA = BodyAComponent->GetRigidBody();
            auto BodyB = BodyBComponent->GetRigidBody();
            auto BodyATransform = CachedEntityA.Transform;
            auto BodyBTransform = CachedEntityB.Transform;
            if (BodyAComponent->IsStatic && BodyBComponent->IsStatic)
            {
                return;
            }

            auto CollisionManifold = CollideCachedEntities<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                CachedEntityA,
                CachedEntityB
            );
            if (CollisionManifold.HasCollided()) {
                CollisionManifoldWithBodies Manifold;
                Manifold.Normal = CollisionManifold.Normal;
                Manifold.Contacts = CollisionManifold.Contacts;
                Manifold.Depth = CollisionManifold.Depth;
                Manifold.FirstRigidBody = BodyAComponent;
                Manifold.SecondRigidBody = BodyBComponent;
                Manifold.FirstCenterOfMassAtCollision = BodyA->GetCenterOfMass(
                    BodyATransform->Scale,
                    BodyATransform->Angle,
                    BodyATransform->Pos
                );
                Manifold.SecondCenterOfMassAtCollision = BodyB->GetCenterOfMass(
                    BodyBTransform->Scale,
                    BodyBTransform->Angle,
                    BodyBTransform->Pos
                );
                CollisionManifolds.push_back(Manifold);

                const auto OverlapResolvingVector = CollisionManifold.Normal * std::max(CollisionManifold.Depth, DistanceStep);
                if (BodyAComponent->IsStatic) {
                    BodyBTransform->Pos = BodyBTransform->Pos + (OverlapResolvingVector * -1);
                    BodyBComponent->GetRigidBody()->IsCacheComputed = false;
                } else if (BodyBComponent->IsStatic) {
                    BodyATransform->Pos = BodyATransform->Pos + OverlapResolvingVector;
                    BodyAComponent->GetRigidBody()->IsCacheComputed = false;
                } else {
                    const auto HalfResolve = OverlapResolvingVector * (1 / 2);
                    BodyATransform->Pos = BodyATransform->Pos + HalfResolve;
                    BodyBTransform->Pos = BodyBTransform->Pos + (HalfResolve * -1);
                    BodyAComponent->GetRigidBody()->IsCacheComputed = false;
                    BodyBComponent->GetRigidBody()->IsCacheComputed = false;
                }
            }
        }

        void ApplyForceFromCollision(
            const CollisionManifoldWithBodies& CollisionManifold
        ) const
        {
            auto BodyAComponent = CollisionManifold.FirstRigidBody;
            auto BodyBComponent = CollisionManifold.SecondRigidBody;
            auto BodyA = BodyAComponent->GetRigidBody();
            auto BodyB = BodyBComponent->GetRigidBody();

            if (BodyAComponent->IsStatic && BodyBComponent->IsStatic)
            {
                return;
            }

            // direction from A to B
            const auto Normal = CollisionManifold.Normal * -1;

            const auto ContactPointCentroid = Maths::GetCentroid(CollisionManifold.Contacts);

            // meters
            const auto VecFromCenterOfMassA = CollisionManifold.FirstCenterOfMassAtCollision.GetVectorTo(ContactPointCentroid) / 100;
            const auto VecFromCenterOfMassB = CollisionManifold.SecondCenterOfMassAtCollision.GetVectorTo(ContactPointCentroid) / 100;

            auto RelativeVelocity = (
                (BodyB->LinearVelocity + VecFromCenterOfMassB * BodyB->AngularVelocity)
                - (BodyA->LinearVelocity + VecFromCenterOfMassA * BodyA->AngularVelocity)
            );
            auto RelativeVelocityOnNormal = RelativeVelocity.Scalar(Normal);

            if (RelativeVelocityOnNormal > typename RigidBodyComponentT::PhysicsT(0))
            {
                RelativeVelocityOnNormal = 0;
            }

            // @todo move this in function
            const auto Restitution = std::min(BodyA->Restitution, BodyB->Restitution);

            const auto DistanceFromCenterOfMassAOnNormal = VecFromCenterOfMassA.Scalar(Normal);
            const auto DistanceFromCenterOfMassBOnNormal = VecFromCenterOfMassB.Scalar(Normal);
            // (-(1 + e)((Va - Vb) ⋅ n)) / ((1 / ma) + (1 / mb) + ((ra ⋅ n)² / Ia) + ((rb ⋅ n)² / Ib))
            const auto jN = (-(typename RigidBodyComponentT::PhysicsT(1) + Restitution) * RelativeVelocityOnNormal)
                            / (
                                BodyA->InvMass + BodyB->InvMass
                                + (DistanceFromCenterOfMassAOnNormal * DistanceFromCenterOfMassAOnNormal * BodyA->InvMomentOfInertiaForZ)
                                + (DistanceFromCenterOfMassBOnNormal * DistanceFromCenterOfMassBOnNormal * BodyB->InvMomentOfInertiaForZ)
                            );

            const auto NormalImpulse = Normal * jN;

            // apply on both bodies
            // (Va2 = Va1 + j / ma ⋅ n)
            BodyA->LinearVelocity = BodyA->LinearVelocity - NormalImpulse * BodyA->InvMass;
            BodyB->LinearVelocity = BodyB->LinearVelocity + NormalImpulse * BodyB->InvMass;

            // (Wa2 = Wa1 + ra ⋅ jn / Ia)
            BodyA->AngularVelocity -= (VecFromCenterOfMassA ^ NormalImpulse) * BodyA->InvMomentOfInertiaForZ;
            BodyB->AngularVelocity += (VecFromCenterOfMassB ^ NormalImpulse) * BodyB->InvMomentOfInertiaForZ;

            // @todo move this in function
            const auto StaticFrictionCoef = (BodyA->StaticFriction + BodyB->StaticFriction) / 2;
            // @todo move this in function
            const auto KineticFrictionCoef = std::clamp(
                (BodyA->KineticFriction + BodyB->KineticFriction) / 2,
                typename RigidBodyComponentT::PhysicsT(0),
                typename RigidBodyComponentT::PhysicsT(1)
            );

            RelativeVelocity = (
                (BodyB->LinearVelocity + VecFromCenterOfMassB * BodyB->AngularVelocity)
                - (BodyA->LinearVelocity + VecFromCenterOfMassA * BodyA->AngularVelocity)
            );
            // Tangent must be in similar direction than velocity
            const auto TangentialVector = (RelativeVelocity - (Normal * RelativeVelocity.Scalar(Normal))).GetNormalized();
            const auto RelativeVelocityOnTangent = Maths::ProjectOnUnitVector(
                RelativeVelocity,
                TangentialVector
            );
            const auto DistanceFromCenterOfMassAOnTangent = VecFromCenterOfMassA.Scalar(TangentialVector);
            const auto DistanceFromCenterOfMassBOnTangent = VecFromCenterOfMassB.Scalar(TangentialVector);

            // (-(1 + e)((Va - Vb) ⋅ -t)) / (1 / ma + 1 / mb + ((ra ⋅ t)² / Ia) + ((rb ⋅ t)² / Ib))
            // where e is the friction coef
            auto jT = (RelativeVelocityOnTangent * -1)
                      / (
                          BodyA->InvMass + BodyB->InvMass
                         + (DistanceFromCenterOfMassAOnTangent * DistanceFromCenterOfMassAOnTangent * BodyA->InvMomentOfInertiaForZ)
                         + (DistanceFromCenterOfMassBOnTangent * DistanceFromCenterOfMassBOnTangent * BodyB->InvMomentOfInertiaForZ)
                      );

            // Coulomb's Law
            typename RigidBodyComponentT::PhysicsVectorT FrictionImpulse;
            if (std::abs(jT) < jN * StaticFrictionCoef) {
                // static friction
                FrictionImpulse = TangentialVector * jT;
            } else {
                // dynamic friction
                // @todo seems to have a little bug because event with friction = 1, the objects moves, angular drag high for now
                FrictionImpulse = TangentialVector * -jN * KineticFrictionCoef;
            }
            BodyA->LinearVelocity = BodyA->LinearVelocity - FrictionImpulse * (BodyA->InvMass);
            BodyB->LinearVelocity = BodyB->LinearVelocity + FrictionImpulse * (BodyB->InvMass);

            BodyA->AngularVelocity -= (VecFromCenterOfMassA ^ FrictionImpulse) * BodyA->InvMomentOfInertiaForZ;
            BodyB->AngularVelocity += (VecFromCenterOfMassB ^ FrictionImpulse) * BodyB->InvMomentOfInertiaForZ;
        }

        template <typename GeometricType, size_t Dimensions>
        Maths::Collisions::CollisionManifold<GeometricType, Dimensions> CollideCachedEntities(
            const CachedEntity& CachedEntityA,
            const CachedEntity& CachedEntityB
        ) const
        {
            auto RigidBodyA = CachedEntityA.Body->GetRigidBody();
            auto RigidBodyB = CachedEntityB.Body->GetRigidBody();

            auto CastedRigidBodyA = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyA);
            auto CastedRigidBodyB = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyB);
            auto ShapeA = dynamic_cast<const Maths::IShape*>(RigidBodyA->GetBoundingBox());
            auto ShapeB = dynamic_cast<const Maths::IShape*>(RigidBodyB->GetBoundingBox());
            if (CastedRigidBodyA && CastedRigidBodyB && ShapeA && ShapeB) {
                if (
                    ShapeA->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                    && ShapeB->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                ) {
                    return Maths::Collisions::SAT::CollideRectangles<GeometricType, Dimensions>(
                        CastedRigidBodyA->GetNormals(),
                        CastedRigidBodyB->GetNormals(),
                        CastedRigidBodyA->GetVertices(),
                        CastedRigidBodyB->GetVertices()
                    );
                }
            }

            throw std::runtime_error("No matching function found");
        }

        template <typename GeometricType, size_t Dimensions>
        bool AreCachedEntitiesLikelyToCollide(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const CachedEntity& EntityA,
            const CachedEntity& EntityB
        ) const
        {
            auto RigidBodyA = EntityA.Body->GetRigidBody();
            auto RigidBodyB = EntityB.Body->GetRigidBody();

            auto CastedRigidBodyA = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyA);
            auto CastedRigidBodyB = dynamic_cast<const Engine::Components::Physics::IRectangleGeometricRigidBody2D<GeometricType>*>(RigidBodyB);
            if (CastedRigidBodyA && CastedRigidBodyB) {
                return Maths::Collisions::SAP::LikelyToCollide<GeometricType, Dimensions>(
                    Axis,
                    CastedRigidBodyA->GetVertices(),
                    CastedRigidBodyB->GetVertices()
                );
            }

            return false;
        }

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
        ) const
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
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IBoundingBox2D<GeometricType>* ShapeA,
            const Maths::IBoundingBox2D<GeometricType>* ShapeB
        ) const
        {
            auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
            auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
            if (CastedShapeA && CastedShapeB) {
                return Maths::Collisions::SAP::LikelyToCollide<GeometricType, Dimensions>(
                    Axis,
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
    };
}
