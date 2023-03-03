//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp>
#include <Engine/Core/Application/EngineApplication.hpp>
#include <Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp>
#include <Engine/UI/Components/Text/TextWidget.hpp>
#include <Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp>
#include <Engine/UI/Components/Sprite/SpriteWidget.hpp>
#include <Engine/AssetLoader/AssetLoader.hpp>

class MainMenuSceneInitializer : public Engine::SceneInitializer
{
public:

    void OnLoaded(Engine::Scene* scene) override
    {
        auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

        auto canvasEntity = scene->CreateEntity();
        auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasWidget->Init();

        auto titreEntity = scene->CreateEntity();
        auto titre = titreEntity.AddComponent<Engine::UI::TextWidget>();
        titre->Init("Worms Like", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 100);

        canvasWidget->AddChild(titre, 0);
        canvasWidget->SetChildOptions(titre, {
            .Anchors = Engine::UI::CanvasAnchors::TOP_CENTER,
            .TopOffset = 20.f,
            .Size = titre->GetSize()
        });

        auto verticalBoxEntity = scene->CreateEntity();
        auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
//        verticalBoxWidget->Init({150, 300}, 100.0f);
        verticalBoxWidget->Init({0, 0}, 100.0f);

        auto buttonPlayEntity = scene->CreateEntity();
        auto buttonPlayWidget = buttonPlayEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonPlayWidget->Init("Play", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 60);
        buttonPlayWidget->SetOnClick([this]() {
            Engine::ScenesSystem::Get()->LoadScene("Game");
        });

        auto buttonSettingsEntity = scene->CreateEntity();
        auto buttonSettingsWidget = buttonSettingsEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonSettingsWidget->Init("Settings", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 60);
        buttonSettingsWidget->SetOnClick([this]() {
            Engine::ScenesSystem::Get()->LoadScene("Options");
        });

        auto buttonExitEntity = scene->CreateEntity();
        auto buttonExitWidget = buttonExitEntity.AddComponent<Engine::UI::TextButtonWidget>();
        buttonExitWidget->Init("Exit", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 60);
        buttonExitWidget->SetOnClick([this]() {
            Engine::EngineApplication::Get()->Stop();
        });

        verticalBoxWidget->AddChild(buttonPlayWidget, 0);
        verticalBoxWidget->AddChild(buttonSettingsWidget, 0);
        verticalBoxWidget->AddChild(buttonExitWidget, 0);

        // @todo set here while UpdatePosition not called on canvas when adding children
        canvasWidget->AddChild(verticalBoxWidget, 0);
        canvasWidget->SetChildOptions(verticalBoxWidget, {
            .Anchors = Engine::UI::CanvasAnchors::CENTER,
            .Size = verticalBoxWidget->GetSize()
        });

        auto backgroundEntity = scene->CreateEntity();
        auto background = backgroundEntity.AddComponent<Engine::UI::SpriteWidget>();
        background->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Background/Background.png"), {0, 0});
        background->SetSize({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

        canvasWidget->AddChild(background, -1);
    }

    void OnUnloaded(Engine::Scene* scene) override
    {

    }

};