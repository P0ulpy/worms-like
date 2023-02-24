//
// Created by Admin on 14/02/2023.
//

#include "MainMenu.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

void MainMenuLayer::OnAttach() {
    auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

    auto scene = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene();
    auto canvasEntity = scene->CreateEntity();
    auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
    canvasWidget->Init({0, 0}, {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

    auto verticalBoxEntity = scene->CreateEntity();
    auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxWidget->Init({70, 70}, 10.0f);

    canvasWidget->AddChild(verticalBoxWidget);

    auto buttonPlayEntity = scene->CreateEntity();
    auto buttonPlayWidget = buttonPlayEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonPlayWidget->Init("Play", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0});

    /*auto buttonSettingsEntity = scene->CreateEntity();
    auto buttonSettingsWidget = buttonSettingsEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonSettingsWidget->Init("Settings", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0});

    auto buttonExitEntity = scene->CreateEntity();
    auto buttonExitWidget = buttonExitEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonExitWidget->Init("Exit", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), {0, 0});*/
    /*buttonExitWidget->SetOnClick([]() {
        Engine::EngineApplication::Get()->GetWindow().close();
    });*/

    verticalBoxWidget->AddChild(buttonPlayWidget);
    /*verticalBoxWidget->AddChild(buttonSettingsWidget);
    verticalBoxWidget->AddChild(buttonExitWidget);*/


}

void MainMenuLayer::OnDetach() {

}

void MainMenuLayer::OnUpdate(Engine::Timestep ts) {

}

void MainMenuLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
