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
#include "Engine/UI/Layout/Grid/WidgetGrid.hpp"
#include "../../UI/Layout/Grid/InventoryPlayer.hpp"
#include "Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.hpp"
#include "Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "Engine/Core/Physics/Physics.hpp"
#include "../Map/Map.hpp"
#include "../Controllers/SidePlayerController.hpp"
#include "../Weapons/Grenade.h"
#include "../Prefabs/GrenadePrefab.h"
#include "../Prefabs/GrenadeFragPrefab.h"

class GameSceneInitializer : public Engine::SceneInitializer
{
public:
    void CreateMethods(Engine::Scene *pScene) {
        SignalSystem::InputSignal::Get()->connect("player_shoot", [pScene]() {
            pScene->InstantiatePrefab<GrenadeFragPrefab>();
        });
    }

    void OnLoaded(Engine::Scene* scene) override
    {
        CreateUI(scene);
        CreateMethods(scene);

        auto Camera = new Engine::Camera::Camera2D<double>();
        Camera->Position = Maths::Point2D<double>(0.f, 0.f);
        scene->SetActiveCamera(Camera);
        scene->AddPhysicsSimulator(new Engine::Physics::Physics2DSimulator<Engine::Components::Physics::RigidBody2DdComponent>());

        auto ControllerEntity = scene->CreateEntity();
        auto Controller = ControllerEntity.AddComponent<Game::Controllers::SidePlayerController>();
        auto PlayerCharacterEntity = scene->CreateEntity();
        auto PlayerCharacter = PlayerCharacterEntity.AddComponent<Game::Actors::PlayerCharacter>();
        auto PlayerTransform = PlayerCharacterEntity.GetComponent<Engine::Components::Transform>();
        Controller->PlayerCharacter = PlayerCharacter;

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

        Controller->MinPosition = Maths::Point2D<double>(0.f, (double) (MapNoiseGenerator.NoiseRangeMax * Map.MinSquareSize * -1));
        Controller->MaxPosition = Maths::Point2D<double>(200 * Map.MinSquareSize, 10.f);

        // @todo compute correctly start player position
        PlayerTransform->Pos = Maths::Point2D<double>(0.f, (double) (MapNoiseGenerator.NoiseRangeMax * Map.MinSquareSize * -1));
//        CreateDebugEntities(scene);
    }

    void OnUnloaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("aba ça game plus padbol");
    }

private:

    SignalSystem::ScopedConnectionSignal m_inventoryConnection;

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

    void CreateUI(Engine::Scene* scene)
    {
        auto windowSize = sf::Vector2f {Engine::EngineApplication::Get()->GetWindow().getSize()};

        auto canvasGameWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetCanvas>();
        canvasGameWidget->Init({0, 0}, {windowSize.x, windowSize.y});

        auto planPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planPlayersWidget->Init({30.0f, windowSize.y - 400.0f}, {500.0f, 500.0f});
        canvasGameWidget->AddChild(planPlayersWidget, 0);

        auto planInventoryWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planInventoryWidget->Init({windowSize.x - 400.0f, windowSize.y - 550.0f}, {500.0f, 500.0f});
        planInventoryWidget->SetActive(false);
        m_inventoryConnection = SignalSystem::InputSignal::Get()->connectScoped("open_inventory", [planInventoryWidget]() {
            if(planInventoryWidget->IsActive())
                planInventoryWidget->SetActive(false);
            else
                planInventoryWidget->SetActive(true);
        });
        canvasGameWidget->AddChild(planInventoryWidget, 0);

        auto verticalBoxPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
        verticalBoxPlayersWidget->Init({0.0f, 0.0f}, 15.0f);
        planPlayersWidget->AddChild(verticalBoxPlayersWidget, 0);

        //Players Health
        for(int i = 0; i < 2; i++)
        {
            auto planHealthPlayerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
            planHealthPlayerWidget->Init({0.0f, 0.0f});
            verticalBoxPlayersWidget->AddChild(planHealthPlayerWidget, 0);

            auto verticalBoxEntity = scene->CreateEntity();
            auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
            verticalBoxWidget->Init({5.0f, 0.0f}, 12.0f);
            verticalBoxWidget->SetSize({96.0f, 60.0f});

            {
                auto textNameWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
                textNameWidget->Init("Name", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::White);
                verticalBoxWidget->AddChild(textNameWidget, 0);

                auto progressBarHealthWidget = scene->CreateEntity().AddComponent<Game::UI::HealthBar>();
                progressBarHealthWidget->Init(
                    {0.0f, 0.0f}, {128.0f, 48.0f}//{96.0f, 36.0f}
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Fillbar_01_Block.png")
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Filler_03.png"));
                verticalBoxWidget->AddChild(progressBarHealthWidget, 0);
            }

            auto backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
            backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
            planHealthPlayerWidget->AddChild(backgroundWidget, -1);

            backgroundWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});
            planHealthPlayerWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});

            planHealthPlayerWidget->AddChild(verticalBoxWidget, 0);
        }

        //Timer
        {
            auto planTimerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
            planTimerWidget->Init({windowSize.x - 200.0f, 0.0f}, {200.0f, 200.0f});
            verticalBoxPlayersWidget->AddChild(planTimerWidget, 0);

            auto horizontalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetHorizontalBox>();
            horizontalBoxWidget->Init({5.0f, 5.0f}, 20.0f);
            planTimerWidget->AddChild(horizontalBoxWidget, 0);

            //Turn Timer
            {
                auto planTimerTurnWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
                planTimerTurnWidget->Init({0.0f, 0.0f}, {100.0f, 100.0f});
                horizontalBoxWidget->AddChild(planTimerTurnWidget, 0);

                auto textTimerTurnValueWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
                textTimerTurnValueWidget->Init("00", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {5.0f, -5.0f}, 60, sf::Color::White);
                planTimerTurnWidget->AddChild(textTimerTurnValueWidget, 0);

                auto backgroundTimerTurnValueWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
                backgroundTimerTurnValueWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_01_Standard.png"), {0.0f, 0.0f});
                planTimerTurnWidget->AddChild(backgroundTimerTurnValueWidget, -1);

                backgroundTimerTurnValueWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
                planTimerTurnWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
            }

            //Global Timer
            {
                auto verticalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
                verticalBoxWidget->Init({0.0f, 0.0f}, 10.0f);
                horizontalBoxWidget->AddChild(verticalBoxWidget, 0);

                auto textTimerGlobalWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
                textTimerGlobalWidget->Init("00:00:00", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0.0f, 0.0f}, 30, sf::Color::White);
                verticalBoxWidget->AddChild(textTimerGlobalWidget, 0);

                //Wind
                {
                    auto planWindWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
                    planWindWidget->Init({0.0f, 0.0f}, {100.0f, 100.0f});
                    verticalBoxWidget->AddChild(planWindWidget, 0);

                    auto WindWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
                    WindWidget->Init("------->", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0.0f, 0.0f}, 30, sf::Color::White);
                    planWindWidget->AddChild(WindWidget, 0);

                    auto backgroundWindWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
                    backgroundWindWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_01_Standard.png"), {-5.0f, 10.0f});
                    planWindWidget->AddChild(backgroundWindWidget, -1);

                    backgroundWindWidget->SetSize({122.5f, 24.0f});
                    planWindWidget->SetSize({122.5f, 24.0f});
                }
            }

            auto backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
            backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
            planTimerWidget->AddChild(backgroundWidget, -1);

            backgroundWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
            planTimerWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
        }

        //Inventory
        {
            auto gridInventoryWidget = scene->CreateEntity().AddComponent<Game::UI::InventoryPlayer>();
            gridInventoryWidget->Init({4, 6}, {2.0f, 2.0f}, {0.0f, 0.0f}
                , Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png")
                , Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_01_Lite.png"));
            gridInventoryWidget->SetCellSize({80.0f, 80.0f});

            planInventoryWidget->AddChild(gridInventoryWidget, 0);

            for(int i = 0; i < 24; i++)
            {
                auto itemWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteButtonWidget>();
                itemWidget->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Items/bomb.png"), {0.0f, 0.0f}, 7.0f);
                gridInventoryWidget->AddChild(itemWidget, 0);
            }
        }
    }
};