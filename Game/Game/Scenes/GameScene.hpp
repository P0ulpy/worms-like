//
// Created by Flo on 28/02/2023.
//

#pragma once

#include "Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

#include "../Prefabs/TestPrefab.hpp"

class GameSceneInitializer : public Engine::SceneInitializer
{
public:

    void OnLoaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("ça game ou quoi ?");

        // Testing purposes
        auto& sceneLayer = App::Get()->GetScenesLayer();
        auto* scene = sceneLayer.GetActiveScene();
        scene->SetActiveCamera(new Engine::Camera::Camera2D<double>());
        scene->AddPhysicsSimulator(new Engine::Physics::Physics2DSimulator<Engine::Components::Physics::RigidBody2DdComponent>());

        Map::Map<200> Map;
        Map::NoiseGenerator MapNoiseGenerator;
        MapNoiseGenerator.NoiseRangeMax = 30.f;
        Map.MinSquareSize = 30.f;
        Map.MinRectangleRatio = 0.15f;
        const auto GenerateMapInThread = [&Map, &MapNoiseGenerator, &scene]() {
            Map.GenerateMapParts(MapNoiseGenerator);
            for (auto& MapPart : Map.MapParts)
            {
                auto MapSquare = scene->CreateEntity();
                auto* transform = MapSquare.AddComponent<Engine::Components::Transform>();
                transform->Pos = MapPart.Position - Maths::Vector2D<double>(600.f, 0.f);
                transform->Pos.GetY() *= -1;

                auto* physics = MapSquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();
                // tmp
                auto MapBoundingBox = MapPart.BoundingBox;
                Maths::RectangleBoundingBox2D<double> BoundingBox;
                BoundingBox.Width = MapBoundingBox.Width;
                BoundingBox.Height = MapBoundingBox.Height;
                BoundingBox.Angle = 0;
                auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
                RigidBody->BoundingBox = BoundingBox;
                physics->SetRigidBody(RigidBody);
                physics->GravityScale = 0.f;
                physics->IsStatic = true;
                RigidBody->Restitution = 0.f;
            }
        };
        // @todo make this work while keeping ref of map
//        std::thread MapGenerationThread(GenerateMapInThread);
        GenerateMapInThread();

//        auto TestBaseSquare = scene->CreateEntity();
//        {
//            auto* transform = TestBaseSquare.AddComponent<Engine::Components::Transform>();
//            transform->Pos = Engine::Components::Transform::PointT(0.f, 400.f);
////        auto* spriteRenderer = TestBaseSquare.AddComponent<Engine::SpriteRenderer>();
////        spriteRenderer->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("../Assets/krab.png"));
//            auto* physics = TestBaseSquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();
//
//            Maths::RectangleBoundingBox2D<double> BoundingBox;
//            BoundingBox.Width = 700.f;
//            BoundingBox.Height = 100.f;
//            BoundingBox.Angle = 30;
//            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
//            RigidBody->BoundingBox = BoundingBox;
//            physics->SetRigidBody(RigidBody);
//            physics->GravityScale = 0.f;
//            // @todo this should not be like this but instead at start of physic compute
//            physics->IsStatic = true;
//            RigidBody->Restitution = 0.f;
//        }

        auto TestGravitySquare = scene->CreateEntity();
        {
            auto* transform = TestGravitySquare.AddComponent<Engine::Components::Transform>();
            transform->Pos = Engine::Components::Transform::PointT(-200.f, -1000.f);
            transform->Scale = Engine::Components::Transform::VectorT (1.f, 1.f);
            auto* physics = TestGravitySquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();

            Maths::RectangleBoundingBox2D<double> BoundingBox;
            BoundingBox.Width = 50.f;
            BoundingBox.Height = 50.f;
            BoundingBox.Angle = 0;
            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBody->BoundingBox = BoundingBox;
            RigidBody->Mass = 0.5f;
            RigidBody->Restitution = 0.f;
            physics->SetRigidBody(RigidBody);
        }
//
//        auto TestGravitySquare2 = scene->CreateEntity();
//        {
//            auto* transform = TestGravitySquare2.AddComponent<Engine::Components::Transform>();
//            transform->Pos = Engine::Components::Transform::PointT(50.f, -400.f);
//            transform->Scale = Engine::Components::Transform::VectorT (2.f, 2.f);
//            auto* physics = TestGravitySquare2.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();
//
//            Maths::RectangleBoundingBox2D<double> BoundingBox;
//            BoundingBox.Width = 75;
//            BoundingBox.Height = 75;
//            // @todo jump on one side, forget meters/cm?
//            BoundingBox.Angle = 30;
//            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
//            RigidBody->BoundingBox = BoundingBox;
//            RigidBody->Mass = 0.1f;
//            physics->SetRigidBody(RigidBody);
//        }
    }

    void OnUnloaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("aba ça game plus padbol");
    }
};