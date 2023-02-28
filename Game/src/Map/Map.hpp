#pragma once

#include <array>
#include <memory>
#include "../Modules/PerlinNoise.hpp"
#include "Engine/RTTI/ClassType.hpp"
#include "Engine/Maths/Vector.hpp"
#include "Engine/Maths/Shape.hpp"
#include "Engine/ECS/EntitiesRegistry/EntitiesRegistry.hpp"

namespace Map
{
    struct NoiseGenerator {
        size_t Octaves = 3;
        float Persistence = 0.5f;
        float StartFrequency = 0.12f;
        float FrequencyModifier = 0.12f;
        float NoiseRangeMin = 0.f;
        float NoiseRangeMax = 50.f;

        static float GetNoiseValue(
            const size_t& X,
            const float& Amplitude,
            const float& Frequency
        )
        {
            return Amplitude * PerlinNoise::db::perlin((float) X * Frequency);
        }

        float SumNoise(const size_t& X) const
        {
            float Amplitude = 1.f;
            float Frequency = StartFrequency;
            float NoiseSum = 0.f;
            float AmplitudeSum = 0.f;

            for (size_t Octave = 0; Octave < Octaves; Octave++)
            {
                NoiseSum += GetNoiseValue(X, Amplitude, Frequency);
                AmplitudeSum += Amplitude;
                Amplitude *= Persistence;
                Frequency *= FrequencyModifier;
            }

            return NoiseSum / AmplitudeSum;
        }

        float RangeMap(float Noise) const
        {
            return NoiseRangeMin + (Noise - -1) * (NoiseRangeMax - NoiseRangeMin) / (1 - -1);
        }

        template <size_t MapSize>
        std::array<float, MapSize> Generate1DNoise() const
        {
            std::array<float, MapSize> Noise;
            for (size_t Idx = 0; Idx < MapSize; Idx++)
            {
                Noise[Idx] = RangeMap(SumNoise(Idx));
            }
            return Noise;
        }
    };

    template <size_t MapSize>
    struct Map
    {
        struct MapPart {
            Maths::RectangleBoundingBox2D<double> BoundingBox;
            Maths::Point2D<double> Position;
        };

        float MinSquareSize = 10.f;
        float MinRectangleRatio = 0.5f;
        std::vector<Engine::EntityHandle> Entities;
        std::vector<MapPart> MapParts;

        void GenerateMapParts(NoiseGenerator Generator)
        {
            Reset();
            const auto Noise = Generator.Generate1DNoise<MapSize>();

            std::vector<MapPart> MapPartsToHandle;
            for (size_t X = 0; X < MapSize; X++) {
                const auto& NoiseValue = Noise[X];
                std::cout << NoiseValue << std::endl;
                for (size_t Y = 0; Y < NoiseValue; Y++)
                {
                    Maths::RectangleBoundingBox2D<double> Rectangle;
                    Rectangle.Height = MinSquareSize;
                    Rectangle.Width = MinSquareSize;
                    auto Pos = Maths::Point2D<double>(MinSquareSize * (double) X, (double) Y * MinSquareSize);
                    MapPartsToHandle.emplace_back(Rectangle, Pos);
                }
            }

//            MapParts = MapPartsToHandle;

//            MapPartsToHandle.clear();
//            for (int i = 0; i < MinSquareSize; i++)
//            {
//                for (int j = 0; j < MinSquareSize; j++)
//                {
//                    Maths::RectangleBoundingBox2D<double> Rectangle;
//                    Rectangle.Height = MinSquareSize;
//                    Rectangle.Width = MinSquareSize;
//                    auto Pos = Maths::Point2D<double>(MinSquareSize * (double) i, (double) j * MinSquareSize);
//                    MapPartsToHandle.emplace_back(Rectangle, Pos);
//                }
//            }

            // @todo maybe extract this
            while (!MapPartsToHandle.empty())
            {
                auto CurrentMapPart = MapPartsToHandle[0];
                MapPartsToHandle.erase(MapPartsToHandle.begin());
                auto Combined = CombineMapParts(CurrentMapPart, MapPartsToHandle);
                MapParts.push_back(Combined);
            }
        }

        MapPart CombineMapParts(
            MapPart& Part,
            std::vector<MapPart>& Parts
        )
        {
            // @todo temp
            const auto& BoundingBox = Part.BoundingBox;
            size_t CurrentCountWidth = BoundingBox.Width / MinSquareSize;
            size_t CurrentCountHeight = BoundingBox.Height / MinSquareSize;

            auto CurrentMinX = Part.Position.GetX() - (BoundingBox.Width / 2);
            auto CurrentMinY = Part.Position.GetY() - (BoundingBox.Height / 2);
            auto CurrentMaxX = std::round(BoundingBox.Width + CurrentMinX);
            auto CurrentMaxY = std::round(BoundingBox.Height + CurrentMinY);

            auto HalfSquareSize = MinSquareSize / 2;
            auto GetMinPos = [HalfSquareSize](const Maths::Point2D<double>& Pos)->Maths::Point2D<double> {
                return Maths::Point2D<double>(
                    Pos.GetX() - HalfSquareSize,
                    Pos.GetY() - HalfSquareSize
                );
            };
            auto CurrentRatio = (float) CurrentCountWidth / (float) CurrentCountHeight;

            auto TryToIncrementHeight = [
                &Part,
                &Parts,
                &GetMinPos,
                &CurrentCountWidth,
                &CurrentMinX,
                &CurrentMaxX,
                &CurrentMinY,
                &CurrentMaxY,
                &HalfSquareSize,
                &CurrentRatio,
                this
            ]()->bool {
                if (CurrentRatio < MinRectangleRatio) return false;
                if (Parts.size() < CurrentCountWidth) return false;
                // sort on Y then X to add a height line
                std::sort(Parts.begin(), Parts.end(), [&GetMinPos, &CurrentMinY](const MapPart& A, const MapPart& B) {
                    auto AMinPos = GetMinPos(A.Position);
                    auto BMinPos = GetMinPos(B.Position);
                    auto AMinY = std::round(AMinPos.GetY());
                    auto BMinY = std::round(BMinPos.GetY());

                    if (AMinY == BMinY) {
                        auto AMinX = std::round(AMinPos.GetX());
                        auto BMinX = std::round(BMinPos.GetX());

                        return AMinX < BMinX;
                    }

                    return AMinY < BMinY;
                });

                auto StartIdx = 0;
                while (GetMinPos(Parts[StartIdx].Position).GetY() <= (CurrentMaxY - MinSquareSize)) {
                    StartIdx++;
                    if (StartIdx == Parts.size()) return false;
                }
                if (Parts.size() - StartIdx < CurrentCountWidth) return false;
                auto LastPart = Parts[StartIdx + CurrentCountWidth - 1];
                auto LastPartPos = GetMinPos(LastPart.Position);
                if (
                    std::round(LastPartPos.GetY()) == CurrentMaxY
                    && std::round(LastPartPos.GetX()) == (CurrentMaxX - MinSquareSize)
                ) {
                    std::cout << "ici: " << std::endl;
                    Parts.erase(Parts.begin() + StartIdx, Parts.begin() + StartIdx + CurrentCountWidth);
                    Part.Position.GetY() += HalfSquareSize;
                    Part.BoundingBox.Height += MinSquareSize;

                    CombineMapParts(Part, Parts);
                    return true;
                }

                std::cout << "là: " << LastPartPos.GetY() << " . " << LastPartPos.GetX() << " | " << CurrentMaxY << " . " << CurrentMaxX - MinSquareSize << std::endl;
                return false;
            };

            auto TryToIncrementWidth = [
                &Part,
                &Parts,
                &GetMinPos,
                &CurrentCountHeight,
                &CurrentMinX,
                &CurrentMaxX,
                &CurrentMinY,
                &CurrentMaxY,
                &HalfSquareSize,
                &CurrentRatio,
                this
            ]()->bool
            {
                if (1 / CurrentRatio < MinRectangleRatio) return false;
                if (Parts.size() < CurrentCountHeight) return false;
                // sort on X then Y to add a width line
                std::sort(Parts.begin(), Parts.end(), [&GetMinPos, &CurrentMinX](const MapPart& A, const MapPart& B) {
                    auto AMinPos = GetMinPos(A.Position);
                    auto BMinPos = GetMinPos(B.Position);
                    auto AMinX = std::round(AMinPos.GetX());
                    auto BMinX = std::round(BMinPos.GetX());

                    if (AMinX == BMinX) {
                        auto AMinY = std::round(AMinPos.GetY());
                        auto BMinY = std::round(BMinPos.GetY());

                        return AMinY < BMinY;
                    }

                    return AMinX < BMinX;
                });

                auto StartIdx = 0;
                while (GetMinPos(Parts[StartIdx].Position).GetX() <= (CurrentMaxX - MinSquareSize)) {
                    StartIdx++;
                    if (StartIdx == Parts.size()) return false;
                }
                if (Parts.size() - StartIdx >= CurrentCountHeight) {
                    auto LastPart = Parts[StartIdx + CurrentCountHeight - 1];
                    auto LastPartPos = GetMinPos(LastPart.Position);
                    if (
                        std::round(LastPartPos.GetY()) == CurrentMaxY - MinSquareSize
                        && std::round(LastPartPos.GetX()) == CurrentMaxX
                        ) {
                        std::cout << "ici 2: " << std::endl;
                        Parts.erase(Parts.begin() + StartIdx, Parts.begin() + StartIdx + CurrentCountHeight);
                        Part.Position.GetX() += HalfSquareSize;
                        Part.BoundingBox.Width += MinSquareSize;

                        CombineMapParts(Part, Parts);
                        return true;
                    } else {
                        std::cout << "là 2: " << LastPartPos.GetY() << " . " << LastPartPos.GetX() << " | " << CurrentMaxY - MinSquareSize << " . " << CurrentMaxX << std::endl;
                    }
                }

                return false;
            };

            // priority to "balance"
            if (CurrentCountWidth > CurrentCountHeight && TryToIncrementHeight()) {
                return Part;
            }

            if (TryToIncrementWidth()) {
                return Part;
            }

            if (TryToIncrementHeight()) {
                return Part;
            }

            return Part;
        }

        void GenerateEntitiesFromMapParts(
            Engine::EntitiesRegistry* Registry
        )
        {
        }

        void Reset()
        {
            // @todo add delete entities ?
        }

        ~Map()
        {
            Reset();
        }
    };
} // Map