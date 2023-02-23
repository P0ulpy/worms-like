#pragma once

#include <vector>
#include <algorithm>
#include "Vector.hpp"
#include "Shape.hpp"

namespace Maths::Collisions
{
    namespace SAT {
        template <typename GeometricType>
        void ComputeAxesAndPointsToCheck(
            const Maths::Vector2D<GeometricType>& ScaleA,
            const Maths::Vector2D<GeometricType>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::Rectangle2D<GeometricType>* RectangleA,
            const Maths::Rectangle2D<GeometricType>* RectangleB,
            std::vector<Maths::Vector2D<GeometricType>>& Axes,
            std::vector<Maths::Point2D<GeometricType>>& PointsA,
            std::vector<Maths::Point2D<GeometricType>>& PointsB
        )
        {
            Axes.clear();
            PointsA.clear();
            PointsB.clear();
            auto VerticesA = RectangleA->GetVertices(RotationDegreesA, ScaleA, OnPosA);
            auto VerticesB = RectangleB->GetVertices(RotationDegreesB, ScaleB, OnPosB);
            auto EdgesA = RectangleA->GetEdges(VerticesA);
            auto EdgesB = RectangleB->GetEdges(VerticesB);
            auto NormalsA = RectangleA->GetNormals(EdgesA);
            auto NormalsB = RectangleB->GetNormals(EdgesB);

            bool Parallel = false;
            for (size_t i = 0; i < NormalsA.size(); i++)
            {
                if (NormalsB[0] == NormalsA[i])
                {
                    Parallel = true;
                    break;
                }
            }

            PointsA.reserve(VerticesA.size());
            PointsB.reserve(VerticesB.size());
            if (Parallel) {
                const size_t ToReserve = std::floor(NormalsA.size() / 2);
                Axes.reserve(ToReserve);
                Axes.push_back(NormalsA[0]);
                Axes.push_back(NormalsA[1]);
            } else {
                Axes.reserve(NormalsA.size() + NormalsB.size());
                Axes.insert(Axes.begin(), NormalsA.begin(), NormalsA.end());
                Axes.insert(Axes.begin(), NormalsB.begin(), NormalsB.end());
            }
            PointsA.insert(PointsA.begin(), VerticesA.begin(), VerticesA.end());
            PointsB.insert(PointsB.begin(), VerticesB.begin(), VerticesB.end());
        }

        template <typename GeometricType, size_t Dimensions>
        void ComputeAxesAndPointsToCheck(
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::Polygon<GeometricType, Dimensions>* PolygonA,
            const Maths::Polygon<GeometricType, Dimensions>* PolygonB,
            std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {

        }

        template <typename GeometricType, size_t Dimensions>
        void GetMinAndMaxProjectedValues(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& Points,
            GeometricType& Min,
            GeometricType& Max
        )
        {
            Min = Max = Maths::ProjectOnUnitVector(Points[0], Axis);
            for (int i = 1; i < Points.size(); i++)
            {
                GeometricType Projection = Maths::ProjectOnUnitVector(Points[i], Axis);
                if (Projection > Max)
                {
                    Max = Projection;
                    continue;
                }
                if (Projection < Min)
                {
                    Min = Projection;
                    continue;
                }
            }
        }

        // OverlapSizeResult must be initialized with < 0 to correctly compute
        template <typename GeometricType, size_t Dimensions>
        bool IntersectOnAxis(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB,
            Maths::Vector<GeometricType, Dimensions>& OverlapDirectionResult,
            GeometricType& OverlapSizeResult
        )
        {
            GeometricType MinA = 0, MaxA = 0, MinB = 0, MaxB = 0;
            GetMinAndMaxProjectedValues(Axis, PointsA, MinA, MaxA);
            GetMinAndMaxProjectedValues(Axis, PointsB, MinB, MaxB);
            if ((MaxA >= MinB) && (MaxB >= MinA))
            {
                GeometricType OverlapSize = std::min(MaxA, MaxB) - std::max(MinA, MinB) + 1;
                if (OverlapSizeResult < 0 || OverlapSize > 0 && OverlapSize < OverlapSizeResult)
                {
                    OverlapSizeResult = std::abs(OverlapSize);
                    OverlapDirectionResult = Axis * (MinA <= MinB && MaxA <= MaxB ? -1 : 1);
                }
                return true;
            }

            return false;
        }

        // TimesOfCollisions must be initialized with +inf, -inf to correctly compute
        template <typename GeometricType, size_t Dimensions>
        bool IntersectOnAxis(
            std::array<float, 2>& TimesOfCollision,
            const Maths::Vector<GeometricType, Dimensions>& VelocityA,
            const Maths::Vector<GeometricType, Dimensions>& VelocityB,
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            GeometricType MinA = 0, MaxA = 0, MinB = 0, MaxB = 0;
            GetMinAndMaxProjectedValues(Axis, PointsA, MinA, MaxA);
            GetMinAndMaxProjectedValues(Axis, PointsB, MinB, MaxB);
            if ((MaxA >= MinB) && (MaxB >= MinA))
            {
                GeometricType OverlapSize = std::min(MaxA, MaxB) - std::max(MinA, MinB) + 1;
                auto RelativeVelocity = Maths::ProjectOnUnitVectorVector(VelocityA, Axis) - Maths::ProjectOnUnitVectorVector(VelocityB, Axis);
                auto RelativeSpeed = RelativeVelocity.GetLength();
                if (RelativeSpeed == 0) {
//                    TimesOfCollision[0] = std::min(TimesOfCollision[0], 0.f);
//                    TimesOfCollision[1] = std::max(TimesOfCollision[1], 0.f);
                } else if (RelativeSpeed > 0) {
                    if ((MinA < MinB < MaxA) || (MinB < MinA < MaxB))
                    {
                        TimesOfCollision[0] = std::min(TimesOfCollision[0], (float) ((MaxA - MinB) / RelativeSpeed));
                        TimesOfCollision[1] = std::max(TimesOfCollision[1], 0.f);
                    } else {
                        TimesOfCollision[0] = std::min(TimesOfCollision[0], (float) ((MaxA - MinB) / RelativeSpeed));
                        TimesOfCollision[1] = std::max(TimesOfCollision[1], (float) ((MinA - MaxB) / RelativeSpeed));
                    }
                } else {
                    if ((MinA < MinB < MaxA) || (MinB < MinA < MaxB))
                    {
                        TimesOfCollision[0] = std::min(TimesOfCollision[0], (float) ((MaxB - MinA) / RelativeSpeed));
                        TimesOfCollision[1] = std::max(TimesOfCollision[1], 0.f);
                    } else {
                        TimesOfCollision[0] = std::min(TimesOfCollision[0], (float) ((MaxB - MinA) / RelativeSpeed));
                        TimesOfCollision[1] = std::max(TimesOfCollision[1], (float) ((MinB - MaxA) / RelativeSpeed));
                    }
                }
                return true;
            }

            return false;
        }

        template <typename GeometricType, size_t Dimensions>
        bool CheckOverlap(
            const std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB,
            Maths::Vector<GeometricType, Dimensions>& OverlapDirectionResult,
            GeometricType& OverlapSizeResult
        )
        {
            for (const auto& Axis : Axes)
            {
                if (!IntersectOnAxis(
                    Axis,
                    PointsA,
                    PointsB,
                    OverlapDirectionResult,
                    OverlapSizeResult
                ))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename GeometricType, size_t Dimensions>
        bool CheckOverlap(
            const std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB,
            Maths::Vector<GeometricType, Dimensions>& ResolvingVector
        )
        {
            Maths::Vector<GeometricType, Dimensions> OverlapDirectionResult;
            GeometricType OverlapSizeResult = -1;
            bool Overlap = CheckOverlap(
                Axes,
                PointsA,
                PointsB,
                OverlapDirectionResult,
                OverlapSizeResult
            );
            ResolvingVector = OverlapDirectionResult * OverlapSizeResult;
            return Overlap;
        }

        template <typename GeometricType, size_t Dimensions>
        bool CheckOverlap(
            std::array<float, 2>& TimesOfCollision,
            const Maths::Vector<GeometricType, Dimensions>& VelocityA,
            const Maths::Vector<GeometricType, Dimensions>& VelocityB,
            const std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            TimesOfCollision = {
                std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity(),
            };
            for (const auto& Axis : Axes)
            {
                if (!IntersectOnAxis(
                    TimesOfCollision,
                    VelocityA,
                    VelocityB,
                    Axis,
                    PointsA,
                    PointsB
                ))
                {
                    return false;
                }
            }
            return true;
        }

        // RTTI casting
        template <typename GeometricType, size_t Dimensions>
        void ComputeAxesAndPointsToCheck(
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IShape* ShapeA,
            const Maths::IShape* ShapeB,
            std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::Rectangle2D<GeometricType>::getClassType())
                )
            {
                return ComputeAxesAndPointsToCheck(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeA),
                    static_cast<const Maths::Rectangle2D<GeometricType>*>(ShapeB),
                    Axes,
                    PointsA,
                    PointsB
                );
            }

            // todo how to detect dimensions for the next ones?
            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::Polygon<GeometricType, Dimensions>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::Polygon<GeometricType, Dimensions>::getClassType())
                )
            {
                return ComputeAxesAndPointsToCheck<GeometricType, Dimensions>(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::Polygon<GeometricType, Dimensions>*>(ShapeA),
                    static_cast<const Maths::Polygon<GeometricType, Dimensions>*>(ShapeB),
                    Axes,
                    PointsA,
                    PointsB
                );
            }

            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::NSphere<GeometricType, Dimensions>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::NSphere<GeometricType, Dimensions>::getClassType())
                )
            {
                return ComputeAxesAndPointsToCheck<GeometricType, Dimensions>(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::NSphere<GeometricType, Dimensions>*>(ShapeA),
                    static_cast<const Maths::NSphere<GeometricType, Dimensions>*>(ShapeB),
                    Axes,
                    PointsA,
                    PointsB
                );
            }

            if (
                ShapeA->getType()->isInstanceOrChildOf(Maths::Polygon<GeometricType, Dimensions>::getClassType())
                && ShapeB->getType()->isInstanceOrChildOf(Maths::NSphere<GeometricType, Dimensions>::getClassType())
                )
            {
                return ComputeAxesAndPointsToCheck<GeometricType, Dimensions>(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::Polygon<GeometricType, Dimensions>*>(ShapeA),
                    static_cast<const Maths::NSphere<GeometricType, Dimensions>*>(ShapeB),
                    Axes,
                    PointsA,
                    PointsB
                );
            }

            throw std::runtime_error("No matching function found.");
        }

        template <typename GeometricType, size_t Dimensions>
        void ComputeAxesAndPointsToCheck(
            const Maths::Vector<GeometricType, Dimensions>& ScaleA,
            const Maths::Vector<GeometricType, Dimensions>& ScaleB,
            const GeometricType& RotationDegreesA,
            const GeometricType& RotationDegreesB,
            const Maths::Point2D<GeometricType>& OnPosA,
            const Maths::Point2D<GeometricType>& OnPosB,
            const Maths::IBoundingBox2D<GeometricType>* ShapeA,
            const Maths::IBoundingBox2D<GeometricType>* ShapeB,
            std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {

            auto CastedShapeA = dynamic_cast<const Maths::IShape*>(ShapeA);
            auto CastedShapeB = dynamic_cast<const Maths::IShape*>(ShapeB);
            if (CastedShapeA && CastedShapeB) {
                return ComputeAxesAndPointsToCheck(
                    ScaleA,
                    ScaleB,
                    RotationDegreesA,
                    RotationDegreesB,
                    OnPosA,
                    OnPosB,
                    static_cast<const Maths::IShape*>(CastedShapeA),
                    static_cast<const Maths::IShape*>(CastedShapeB),
                    Axes,
                    PointsA,
                    PointsB
                );
            }

            throw std::runtime_error("No matching function found.");
        }
    }
    // @todo maybe implement other types than SAT, like SAP and Impulse
} // Collisions // Maths