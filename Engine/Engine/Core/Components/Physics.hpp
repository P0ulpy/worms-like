#pragma once

#include "../../Maths/Shape.hpp"
#include "../../ECS/Component/Component.hpp"
#include "Transform.hpp"
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
        PhysicsT Torque;
        // kg
        PhysicsT Mass = 1;
        PhysicsT MomentOfInertia;

        virtual ~IRigidBody() = default;
    };

    template <typename GeometricT, typename PhysicsT>
    struct IRigidBody2D : public IRigidBody<2, GeometricT, PhysicsT>
    {
        using IBoundingBox2D = Maths::IBoundingBox2D<GeometricT>;
        virtual IBoundingBox2D* GetBoundingBox() = 0;
        virtual void ComputeInertia() = 0;
        virtual void ComputeForceAndTorque(const bool& HasGravity, const Maths::Vector2D<PhysicsT>& GravityVec) = 0;
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
        void ComputeInertia()
        {
            const auto mass = this->Mass;
            const auto w = this->BoundingBox.Width;
            const auto h = this->BoundingBox.Height;
            this->MomentOfInertia = mass * (w * w + h * h) / 12;
        }

        void ComputeForceAndTorque(const bool& HasGravity, const Maths::Vector2D<PhysicsT>& GravityVec)
        {
            if (HasGravity)
            {
                this->Force += GravityVec;
            }
            const auto r = Maths::Vector2D<PhysicsT>(this->BoundingBox.Width / 2, this->BoundingBox.Height / 2);
            this->Torque = r.Scalar(this->Force);
        }
    };

    using RectangleRigidBody2Dd = RectangleRigidBody2D<double, double>;

    template <typename GT, typename PT, bool Debug = false>
    class RigidBody2DComponent : public Engine::Component
    {
    public:
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
            m_RigidBodies = Bodies;
            for (auto & Body : m_RigidBodies)
            {
                ComputeRigidBodyInertia(Body);
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
                    Rect.setSize({(float)RectangleBoundingBox->Width, (float)RectangleBoundingBox->Height});
                    const auto FinalPos = EntityTransform->Pos + GeometricVector2DT(
                        RectangleBoundingBox->Position.GetX(),
                        RectangleBoundingBox->Position.GetY()
                    );
                    Rect.setPosition({
                         (float) FinalPos.GetX(),
                         (float) FinalPos.GetY(),
                    });
                    Rect.setFillColor(sf::Color::Transparent);
                    Rect.setOutlineColor(sf::Color::White);
                    Rect.setOutlineThickness(1.f);
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
                    ComputeForceAndTorque(StepMs, rigidBody);
                    const PhysicsVector2DT linearAcceleration = PhysicsVector2DT(
                        rigidBody->Force.GetX() / rigidBody->Mass,
                        rigidBody->Force.GetY() / rigidBody->Mass
                    );
                    auto BoundingBox = rigidBody->GetBoundingBox();
                    auto PosVec = GeometricVector2DT{BoundingBox->Position.Values};
                    EntityTransform->Pos.GetX() += rigidBody->LinearVelocity.GetX() * StepMs;
                    EntityTransform->Pos.GetY() += rigidBody->LinearVelocity.GetY() * StepMs;
                    rigidBody->LinearVelocity.GetX() += linearAcceleration.GetX() * StepMs;
                    rigidBody->LinearVelocity.GetY() += linearAcceleration.GetY() * StepMs;
                    const auto angularAcceleration = rigidBody->Torque / rigidBody->MomentOfInertia;
                    BoundingBox->Angle += rigidBody->AngularVelocity * StepMs;
                    rigidBody->AngularVelocity += angularAcceleration * StepMs;
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

        bool IsPointInside(const Maths::Point2D<GeometricT>& Point) const
        {
            IRigidBody2DT*& IgnoredInRigidBody = nullptr;
            return IsPointInside(Point, IgnoredInRigidBody);
        }

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

        std::vector<GeometricPoint2DT> GetProjectablePoints() const
        {
            const auto Transform = GetEntityTransform();
            const auto PosVec = Transform::VectorT{Transform->Pos.Values};
            std::vector<GeometricPoint2DT> Points;
            for (const auto& Body : m_RigidBodies)
            {
                const auto BodyPoints = Body->GetBoundingBox()->GetProjectablePoints();
                for (const auto& P : BodyPoints) {
                    Points.push_back(P + PosVec);
                }
            }
            return Points;
        }

        std::vector<GeometricVector2DT> GetAxes() const
        {
            std::vector<GeometricVector2DT> Axes;
            for (const auto& Body : m_RigidBodies)
            {
                const auto BodyAxes = Body->GetBoundingBox()->GetAxes();
                Axes.insert(Axes.end(), BodyAxes.begin(), BodyAxes.end());
            }
            return Axes;
        }

        std::vector<IRigidBody2DT*>& GetRigidBodies()
        {
            return m_RigidBodies;
        }
    private:
        // @todo can be useful to have multiple rigidbodies but will not work with this implementation bc of gravity?
        std::vector<IRigidBody2DT*> m_RigidBodies;

        static void ComputeRigidBodyInertia(IRigidBody2DT* RigidBody)
        {
            RigidBody->ComputeInertia();
        }

        void ComputeForceAndTorque(const float& StepMs, IRigidBody2DT* RigidBody)
        {
            const auto GravityVec = GravityDirection * Gravity * (StepMs / 1000);
            RigidBody->ComputeForceAndTorque(HasGravity, GravityVec);
        }

        [[nodiscard]] Engine::Components::Transform* GetEntityTransform() const
        {
            return GetEntity().template GetComponent<Engine::Components::Transform>();
        }
    };

    using RigidBody2DdComponent = RigidBody2DComponent<double, double, true>;
}