//
// Created by Admin on 14/02/2023.
//

#include "MainMenu.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "Engine/UI/Components/Sprite/SpriteWidget.hpp"
#include "Engine/UI/Components/Text/TextWidget.hpp"
#include "OptionMenu.hpp"

void MainMenuLayer::OnAttach() {
    auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

    auto scene = Engine::EngineApplication::Get()->GetScenesLayer()->GetActiveScene();
    auto canvasEntity = scene->CreateEntity();
    auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
    canvasWidget->Init({0, 0}, {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

    auto titre = canvasEntity.AddComponent<Engine::UI::TextWidget>();
    titre->Init("Worms Like", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0}, 100);
    auto x = (static_cast<float>(windowSize.x) / 2) - (titre->GetSize().x / 2);
    titre->SetPosition({ x, 100.0f });

    canvasWidget->AddChild(titre);

    auto verticalBoxEntity = scene->CreateEntity();
    auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxWidget->Init({150, 300}, 100.0f);

    canvasWidget->AddChild(verticalBoxWidget);

    auto buttonPlayEntity = scene->CreateEntity();
    auto buttonPlayWidget = buttonPlayEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonPlayWidget->Init("Play", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0}, 0.0f, 60);
    buttonPlayWidget->SetOnClick([]() {
        //Play Game
    });

    auto buttonSettingsEntity = scene->CreateEntity();
    auto buttonSettingsWidget = buttonSettingsEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonSettingsWidget->Init("Settings", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0}, 0.0f, 60);
    buttonSettingsWidget->SetOnClick([this]() {
        OptionMenuLayer options;
        Engine::EngineApplication* app = Engine::EngineApplication::Get();
        app->RemoveLayer(this);
        app->PushLayer(&options);
    });

    auto buttonExitEntity = scene->CreateEntity();
    auto buttonExitWidget = buttonExitEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonExitWidget->Init("Exit", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0}, 0.0f, 60);
    buttonExitWidget->SetOnClick([]() {
        Engine::EngineApplication::Get()->GetWindow().close();
    });

    verticalBoxWidget->AddChild(buttonPlayWidget);
    verticalBoxWidget->AddChild(buttonSettingsWidget);
    verticalBoxWidget->AddChild(buttonExitWidget);

    /*auto background = canvasEntity.AddComponent<Engine::UI::SpriteWidget>();
    background->Init(*Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/Background.png"), {0, 0});
    background->SetSize({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

    canvasWidget->AddChild(background);*/
}

void MainMenuLayer::OnDetach() {

}

void MainMenuLayer::OnUpdate(Engine::Timestep ts) {

}

void MainMenuLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
