//
// Created by Admin on 27/02/2023.
//

#include "ConfigGameMenu.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "SFML/Graphics/Font.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Game.hpp"

void ConfigGameMenuLayer::OnAttach() {
    auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

    auto scene = Engine::EngineApplication::Get()->GetScenesLayer()->GetActiveScene();
    auto canvasEntity = scene->CreateEntity();
    auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
    canvasWidget->Init({0, 0}, {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

    auto buttonPlayEntity = scene->CreateEntity();
    auto buttonPlayWidget = buttonPlayEntity.AddComponent<Engine::UI::TextButtonWidget>();
    buttonPlayWidget->Init("Play", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0, 0}, 0.0f, 80);
    auto xBack = (static_cast<float>(windowSize.x) / 2) - (buttonPlayWidget->GetSize().x / 2);
    auto yBack = static_cast<float>(windowSize.y) - buttonPlayWidget->GetSize().y - 100.0f;
    buttonPlayWidget->SetPosition({ xBack, yBack });
    buttonPlayWidget->SetOnClick([this]() {
        Engine::Logger::Log("Load Game Scene");
        //Engine::EngineApplication::Get()->RemoveLayer(this);
        //GameLayer gameLayer;
        //Engine::EngineApplication::Get()->PushLayer(&gameLayer);
    });

    canvasWidget->AddChild(buttonPlayWidget);
}

void ConfigGameMenuLayer::OnDetach() {

}

void ConfigGameMenuLayer::OnUpdate(Engine::Timestep ts) {

}

void ConfigGameMenuLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
