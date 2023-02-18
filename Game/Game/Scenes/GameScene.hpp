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
//        Engine::EngineApplication::Get()->GetWindow().close();

        auto& sceneLayer = App::Get()->GetScenesLayer();
        auto* scene = Engine::ScenesSystem::Get();

        auto TestGravitySquare = scene->CreateEntity();
        {
            auto* transform = TestGravitySquare.AddComponent<Engine::Components::Transform>();
            transform->Pos = Engine::Components::Transform::PointT(100.f, 100.f);
//        auto* spriteRenderer = TestGravitySquare.AddComponent<Engine::SpriteRenderer>();
//        spriteRenderer->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("../Assets/krab.png"));
            auto* physics = TestGravitySquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();

            Maths::RectangleBoundingBox2D<double> BoundingBox;
            BoundingBox.Width = 50;
            BoundingBox.Height = 50;
            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBody->BoundingBox = BoundingBox;
            physics->SetRigidBodies({RigidBody});
        }

        auto TestBaseSquare = scene->CreateEntity();
        {
            auto* transform = TestBaseSquare.AddComponent<Engine::Components::Transform>();
            transform->Pos = Engine::Components::Transform::PointT(50.f, 500.f);
//        auto* spriteRenderer = TestBaseSquare.AddComponent<Engine::SpriteRenderer>();
//        spriteRenderer->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("../Assets/krab.png"));
            auto* physics = TestBaseSquare.AddComponent<Engine::Components::Physics::RigidBody2DdComponent>();

            Maths::RectangleBoundingBox2D<double> BoundingBox;
            BoundingBox.Width = 600;
            BoundingBox.Height = 50;
            auto RigidBody = new Engine::Components::Physics::RectangleRigidBody2Dd();
            RigidBody->BoundingBox = BoundingBox;
            physics->SetRigidBodies({RigidBody});
            physics->HasGravity = false;
            physics->IsMovable = false;
        }
    }

    void OnUnloaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("aba ça game plus padbol");
    }
};