//
// Created by Flo on 28/02/2023.
//

#pragma once

#include "Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

#include "../Prefabs/TestPrefab.hpp"
#include "Engine/UI/Layout/Grid/WidgetGrid.hpp"
#include "../../UI/Layout/Grid/InventoryPlayer.hpp"
#include "Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.hpp"
#include "Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "Engine/Core/Physics/Physics.hpp"
#include "../Map/Map.hpp"
#include "../Controllers/SidePlayerController.hpp"
#include "../GameSystems/GameManagerPrefab.hpp"
#include "../Weapons/Grenade.h"
#include "../Prefabs/GrenadePrefab.h"
#include "../Prefabs/GrenadeFragPrefab.h"
#include "../GameSystems/PhysicsEvents/PhysicsEvents.hpp"

/*#include "Maths/Vector.hpp"*/

class GameSceneInitializer : public Engine::SceneInitializer
{
public:
    void CreateMethods(Engine::Scene *pScene) {
        /*SignalSystem::InputSignal::Get()->connect("player_shoot", [pScene]() {
            pScene->InstantiatePrefab<GrenadeFragPrefab>();
        });*/
    }

    void OnLoaded(Engine::Scene* scene) override
    {
        CreateMethods(scene);
        CreateGameSystems(scene);

        auto Camera = new Engine::Camera::Camera2D<double>();
        Camera->Position = Maths::Point2D<double>(0.f, 0.f);
        scene->SetActiveCamera(Camera);
        scene->AddPhysicsSimulator(new Engine::Physics::Physics2DSimulator<Engine::Components::Physics::RigidBody2DdComponent>());

        Map::Map<200> Map;
        Map::NoiseGenerator MapNoiseGenerator;
        MapNoiseGenerator.NoiseRangeMax = 30.f;
        Map.MinSquareSize = 30.f;
        Map.MinRectangleRatio = 0.15f;
        const auto GenerateMapInThread = [&Map, &MapNoiseGenerator, &scene]() {
            Engine::Logger::Log("Generating map...");
            Map.GenerateMapParts(MapNoiseGenerator);
            for (auto& MapPart : Map.MapParts)
            {
                auto MapSquare = scene->CreateEntity();
                auto* transform = MapSquare.AddComponent<Engine::Components::Transform>();
                transform->Pos = MapPart.Position;
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
            }
            Engine::Logger::Log("Map generated");
        };
        // @todo make this work while keeping ref of map
//        std::thread MapGenerationThread(GenerateMapInThread);
        GenerateMapInThread();

        // @todo compute correctly start player position
        //PlayerTransform->Pos = Maths::Point2D<double>(0.f, (double) (MapNoiseGenerator.NoiseRangeMax * Map.MinSquareSize * -1));
//        CreateDebugEntities(scene);

        auto WindApplierEntity = scene->CreateEntity();
        auto* windApplier = WindApplierEntity.AddComponent<Game::PhysicsEvents::WindApplier>();
    }

    void OnUnloaded(Engine::Scene* scene) override
    {

    }

private:

    void CreateDebugEntities(Engine::Scene* Scene) const
    {
        auto TestBaseSquare = Scene->CreateEntity();
        {
            auto* transform = TestBaseSquare.AddComponent<Engine::Components::Transform>();
            transform->Pos = Engine::Components::Transform::PointT(0.f, 400.f);
            auto* physics = TestBaseSquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();

            Maths::RectangleBoundingBox2D<double> BoundingBox;
            BoundingBox.Width = 700.f;
            BoundingBox.Height = 100.f;
            BoundingBox.Angle = 10;
            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBody->BoundingBox = BoundingBox;
            physics->SetRigidBody(RigidBody);
            physics->GravityScale = 0.f;
            physics->IsStatic = true;
        }

        auto TestGravitySquare = Scene->CreateEntity();
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
            physics->SetRigidBody(RigidBody);
        }

        auto TestGravitySquare2 = Scene->CreateEntity();
        {
            auto* transform = TestGravitySquare2.AddComponent<Engine::Components::Transform>();
            transform->Pos = Engine::Components::Transform::PointT(50.f, -400.f);
            transform->Scale = Engine::Components::Transform::VectorT (2.f, 2.f);
            auto* physics = TestGravitySquare2.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();

            Maths::RectangleBoundingBox2D<double> BoundingBox;
            BoundingBox.Width = 75;
            BoundingBox.Height = 75;
            BoundingBox.Angle = 30;
            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBody->BoundingBox = BoundingBox;
            RigidBody->Mass = 0.1f;
            physics->SetRigidBody(RigidBody);
        }
    }

    void CreateGameSystems(Engine::Scene* scene)
    {
        scene->InstantiatePrefab<GameManagerPrefab>();
    }
};