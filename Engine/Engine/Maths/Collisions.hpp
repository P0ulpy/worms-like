#pragma once

#include <vector>
#include "Vector.hpp"
#include "Shape.hpp"

namespace Maths::Collisions
{
    namespace SAT {
        template <typename GeometricType, size_t Dimensions>
        void ComputeAxesAndPointsToCheck(
            const Maths::Polygon<GeometricType, Dimensions> PolygonA,
            const Maths::Polygon<GeometricType, Dimensions> PolygonB,
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

        template <typename GeometricType, size_t Dimensions>
        bool IntersectOnAxis(
            const Maths::Vector<GeometricType, Dimensions>& Axis,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            GeometricType MinA = 0, MaxA = 0, MinB = 0, MaxB = 0;
            GetMinAndMaxProjectedValues(Axis, PointsA, MinA, MaxA);
            GetMinAndMaxProjectedValues(Axis, PointsB, MinB, MaxB);
            return (MaxA >= MinB) && (MaxB >= MinA);
        }

        template <typename GeometricType, size_t Dimensions>
        bool CheckOverlap(
            const std::vector<Maths::Vector<GeometricType, Dimensions>>& Axes,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsA,
            const std::vector<Maths::Point<GeometricType, Dimensions>>& PointsB
        )
        {
            for (const auto& Axis : Axes)
            {
                if (!IntersectOnAxis(Axis, PointsA, PointsB))
                {
                    return false;
                }
            }
            return true;
        }
    }
} // Collisions // Maths