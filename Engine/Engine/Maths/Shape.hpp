#pragma once

#include <vector>
#include "Vector.hpp"

namespace Maths {
    struct IShape
    {
        virtual ~IShape() = default;
    };

    template <typename T, size_t Dimensions>
    struct Polygon : IShape
    {
        virtual std::vector<Point2D<T>> GetVertices() const = 0;

        std::vector<Vector<T, Dimensions>> GetEdges() const
        {
            auto Vertices = GetVertices();
            std::vector<Vector<T, Dimensions>> Edges = {};
            Edges.resize(Vertices.size());
            for (size_t i = 1; i <= Vertices.size(); i++)
            {
                const auto CurrentIdx = i % (Vertices.size());
                Edges[CurrentIdx] = Vertices[i - 1].GetVector(Vertices[CurrentIdx]);
            }
            return Edges;
        }

        std::vector<Vector<T, Dimensions>> GetNormals() const
        {
            static const auto UnitAxisVector = Vector<T, Dimensions>::GetUnitVectorOnAxis(0);
            auto Normals = GetEdges();
            for (size_t i = 0; i < Normals.size(); i++)
            {
                if constexpr (Dimensions == 2)
                {
                    Normals[i] = Vector<T, Dimensions>(-Normals[i].GetY(), Normals[i].GetX());
                } else {
                    Normals[i] = Normals[i] ^ UnitAxisVector;
                }
                Normals[i].Normalize();
            }

            return Normals;
        }

        std::vector<Vector<T, Dimensions>> GetAxes() const
        {
            return GetNormals();
        }
    };

    template <typename T>
    struct Rectangle2D : public Polygon<T, 2>
    {
        T Width;
        T Height;

        T ComputeMaxDistanceFromPosition()
        {
            T HalfHeight = Height / 2;
            T HalfWidth = Width / 2;
            // half diagonal
            return std::sqrt(HalfWidth * HalfWidth + HalfHeight * HalfHeight);
        }

        std::vector<Point2D<T>> GetVertices() const override
        {
            return {
                Point2D<T>(T(0), T(0)),
                Point2D<T>(Width, T(0)),
                Point2D<T>(Width, Height),
                Point2D<T>(T(0), Height),
            };
        }

        std::vector<Point2D<T>> GetProjectablePoints()
        {
            return GetVertices();
        }

        virtual bool IsPointInside(const Point2D<T>& Point) const
        {
            return Point.GetX() >= 0
                && Point.GetX() <= Width
                && Point.GetY() >= 0
                && Point.GetY() <= Height
            ;
        }
    };

    template <typename T>
    struct IBoundingBox2D
    {
        Point2D<T> Position = Point2D<T>(T(0), T(0));
        float Angle = 0;

        virtual std::vector<Vector2D<T>> GetAxes() = 0;
        virtual std::vector<Point2D<T>> GetProjectablePoints() = 0;
        virtual T ComputeMaxDistanceFromPosition() = 0;
        virtual ~IBoundingBox2D() = default;
    };

    template <typename T, template <typename ShapeT> typename Shape>
    struct BoundingBox2D : public Shape<T>, public IBoundingBox2D<T>
    {
        bool IsPointInside(const Point2D<T>& Point) const override
        {
            return Shape<T>::IsPointInside(Point + Vector2D<T>(
                this->Position.GetX(),
                this->Position.GetY()
            ));
        }

        // @todo handle angle
        std::vector<Vector2D<T>> GetAxes() override
        {
            return Shape<T>::GetAxes();
        }

        // @todo handle angle
        std::vector<Point2D<T>> GetProjectablePoints() override
        {
            auto Points = Shape<T>::GetProjectablePoints();
//            const auto VecFromPos = Vector2D<T>{this->Position.Values};
//            for (size_t i = 0; i < Points.size(); i++)
//            {
//                Points[i] = Points[i] + VecFromPos;
//            }
            return Points;
        }

        T ComputeMaxDistanceFromPosition() override
        {
            return Shape<T>::ComputeMaxDistanceFromPosition();
        }
    };

    template <typename T>
    using RectangleBoundingBox2D = BoundingBox2D<T, Rectangle2D>;
};
