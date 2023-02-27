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
    public:
        void Simulate(
            const float& DeltaTime,
            Engine::EntitiesRegistry* EntitiesRegistry
        ) override
        {
            std::vector<RigidBodyComponentT*> Bodies;
            for (auto& [Handle, Component] : EntitiesRegistry->GetSystem<RigidBodyComponentT>()->components)
            {
                Bodies.push_back(&Component);
            }

            SimulateRigidBodies(DeltaTime, Bodies);

            HandleCollisionsOnBodies(Bodies);
        }

        RTTI::ClassType* GetBodyType() override
        {
            return RigidBodyComponentT::getClassType();
        }
    private:
        void SimulateRigidBodies(
            const float& DeltaTime,
            std::vector<RigidBodyComponentT*>& Bodies
        ) const
        {
            for (auto& BodyComponent : Bodies) {
                BodyComponent->SimulateRigidBody(DeltaTime);
            }
        }

        void HandleCollisionsOnBodies(
            std::vector<RigidBodyComponentT*> Bodies
        ) const
        {
            // choose axis Y because with gravity it should be the most reliable for performance
            const auto Axis = RigidBodyComponentT::GeometricVectorT::GetUnitVectorOnAxis(1);
            std::sort(Bodies.begin(), Bodies.end(), [&Axis](RigidBodyComponentT* A, RigidBodyComponentT* B)->bool {
                return Maths::ProjectOnUnitVector(A->GetEntityTransform()->Pos, Axis) < Maths::ProjectOnUnitVector(B->GetEntityTransform()->Pos, Axis);
            });

            std::vector<std::array<RigidBodyComponentT*, 2>> LikelyToCollide;
            std::vector<RigidBodyComponentT*> ActiveIntervals;
            for (auto& Body : Bodies)
            {
                auto BodyTransform = Body->GetEntityTransform();

                for (auto BodyIt = ActiveIntervals.rbegin(); BodyIt < ActiveIntervals.rend(); BodyIt++) {
                    auto WithTransform = (*BodyIt)->GetEntityTransform();
                    if (AreBoundingBoxLikelyToCollide<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                        Axis,
                        BodyTransform->Scale,
                        WithTransform->Scale,
                        BodyTransform->Angle,
                        WithTransform->Angle,
                        BodyTransform->Pos,
                        WithTransform->Pos,
                        Body->GetRigidBody()->GetBoundingBox(),
                        (*BodyIt)->GetRigidBody()->GetBoundingBox()
                    )) {
                        LikelyToCollide.push_back({Body, *BodyIt});
                    } else {
                        // we do not need to keep it as we are only going further
                        ActiveIntervals.erase(std::next(BodyIt).base());
                    }
                }
                ActiveIntervals.push_back(Body);
            }

            std::vector<CollisionManifoldWithBodies> ContactManifolds;
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
            RigidBodyComponentT* BodyAComponent,
            RigidBodyComponentT* BodyBComponent,
            std::vector<CollisionManifoldWithBodies>& CollisionManifolds
        ) const
        {
            auto BodyA = BodyAComponent->GetRigidBody();
            auto BodyB = BodyBComponent->GetRigidBody();
            auto BodyATransform = BodyAComponent->GetEntity().template GetComponent<Engine::Components::Transform>();
            auto BodyBTransform = BodyBComponent->GetEntity().template GetComponent<Engine::Components::Transform>();
            if (BodyAComponent->IsStatic && BodyBComponent->IsStatic)
            {
                return;
            }

            auto CollisionManifold = CollideBoundingBoxes<typename RigidBodyComponentT::GeometricT, RigidBodyComponentT::Dimensions>(
                BodyATransform->Scale,
                BodyBTransform->Scale,
                BodyATransform->Angle,
                BodyBTransform->Angle,
                BodyATransform->Pos,
                BodyBTransform->Pos,
                BodyA->GetBoundingBox(),
                BodyB->GetBoundingBox()
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
                } else if (BodyBComponent->IsStatic) {
                    BodyATransform->Pos = BodyATransform->Pos + OverlapResolvingVector;
                } else {
                    const auto HalfResolve = OverlapResolvingVector * (1 / 2);
                    BodyATransform->Pos = BodyATransform->Pos + HalfResolve;
                    BodyBTransform->Pos = BodyBTransform->Pos + (HalfResolve * -1);
                }
            }
        }

        void ApplyForceFromCollision(
            CollisionManifoldWithBodies CollisionManifold
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

// old:            auto RelativeVelocity = BodyB->LinearVelocity - BodyA->LinearVelocity;
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

            // meters
            const auto DistanceFromCenterOfMassAOnNormal = VecFromCenterOfMassA ^ Normal;
            const auto DistanceFromCenterOfMassBOnNormal = VecFromCenterOfMassB ^ Normal;
            // (-(1 + e)((Va - Vb) ⋅ n)) / ((1 / ma) + (1 / mb) + ((ra ^ n)² / Ia) + ((rb ^ n)² / Ib))
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
            BodyA->AngularVelocity -= jN * DistanceFromCenterOfMassAOnNormal * BodyA->InvMomentOfInertiaForZ;
            BodyB->AngularVelocity += jN * DistanceFromCenterOfMassBOnNormal * BodyB->InvMomentOfInertiaForZ;

            // @todo move this in function
            const auto StaticFrictionCoef = (BodyA->StaticFriction + BodyB->StaticFriction) / 2;
            // @todo move this in function
            const auto KineticFrictionCoef = std::clamp(
                (BodyA->KineticFriction + BodyB->KineticFriction) / 2,
                typename RigidBodyComponentT::PhysicsT(0),
                typename RigidBodyComponentT::PhysicsT(1)
            );

// old:            RelativeVelocity = (BodyB->LinearVelocity - BodyA->LinearVelocity);
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
            const auto DistanceFromCenterOfMassAOnTangent = VecFromCenterOfMassA ^ TangentialVector;
            const auto DistanceFromCenterOfMassBOnTangent = VecFromCenterOfMassB ^ TangentialVector;

            // (-(1 + e)((Va - Vb) ⋅ -t)) / (1 / ma + 1 / mb + ((ra x n)² / Ia) + ((rb x n)² / Ib))
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
                FrictionImpulse = TangentialVector * -jN * KineticFrictionCoef;
            }
            BodyA->LinearVelocity = BodyA->LinearVelocity - FrictionImpulse * (BodyA->InvMass);
            BodyB->LinearVelocity = BodyB->LinearVelocity + FrictionImpulse * (BodyB->InvMass);

            // fix better with threshold
            {
                auto Velocity = jT * DistanceFromCenterOfMassAOnTangent * BodyA->InvMomentOfInertiaForZ;
//                if (std::abs(Velocity) > BodyA->AngularVelocityThreshold) {
                    BodyA->AngularVelocity -= Velocity;
//                }
            }

            {
                auto Velocity = jT * DistanceFromCenterOfMassBOnTangent * BodyB->InvMomentOfInertiaForZ;
//                if (std::abs(Velocity) > BodyB->AngularVelocityThreshold) {
                    BodyB->AngularVelocity += Velocity;
//                }
            }
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
