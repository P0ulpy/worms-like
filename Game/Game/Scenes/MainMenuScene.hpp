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

class MainMenuSceneInitializer : public Engine::SceneInitializer
{
public:

    void OnLoaded(Engine::Scene* scene) override
    {
        auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

        auto TestPrefabEntity = scene->InstantiatePrefab<TestPrefab>();

        auto canvasEntity = scene->CreateEntity();
        auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasWidget->Init({0, 0}, {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

        auto verticalBoxEntity = scene->CreateEntity();
        auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
        verticalBoxWidget->Init({70, 70}, 10.0f);

        canvasWidget->AddChild(verticalBoxWidget);

        auto buttonPlayEntity = scene->CreateEntity();
        auto buttonPlayWidget = buttonPlayEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonPlayWidget->Init("Play", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font.otf"), {0, 0});
        buttonPlayWidget->SetOnClick([]()
        {
            Engine::ScenesSystem::Get()->LoadScene("Game");
        });

        auto buttonSettingsEntity = scene->CreateEntity();
        auto buttonSettingsWidget = buttonSettingsEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonSettingsWidget->Init("Settings", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font.otf"), {0, 0});

        auto buttonExitEntity = scene->CreateEntity();
        auto buttonExitWidget = buttonExitEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonExitWidget->Init("Exit", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font.otf"), {0, 0});
        buttonExitWidget->SetOnClick([]() {
            Engine::EngineApplication::Get()->GetWindow().close();
        });

        verticalBoxWidget->AddChild(buttonPlayWidget);
        verticalBoxWidget->AddChild(buttonSettingsWidget);
        verticalBoxWidget->AddChild(buttonExitWidget);
    }

    void OnUnloaded(Engine::Scene* scene) override
    {

    }

};