//
// Created by Admin on 27/02/2023.
//

#include "Game.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Text/TextWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "../UI/Components/ProgressBar/HealthBar.hpp"
#include "Engine/UI/Components/Sprite/SpriteWidget.hpp"

void GameLayer::OnAttach() {
    auto windowSize = sf::Vector2f {Engine::EngineApplication::Get()->GetWindow().getSize()};

    auto scene = Engine::EngineApplication::Get()->GetScenesLayer()->GetActiveScene();
    auto canvasPlayersEntity = scene->CreateEntity();
    auto canvasPlayersWidget = canvasPlayersEntity.AddComponent<Engine::UI::WidgetCanvas>();
    canvasPlayersWidget->Init({30.0f, windowSize.y - 400.0f}, {500.0f, 500.0f});

    /*auto canvasInventoryEntity = scene->CreateEntity();
    auto canvasInventoryWidget = canvasInventoryEntity.AddComponent<Engine::UI::WidgetCanvas>();
    canvasInventoryWidget->Init({windowSize.x - 500.0f, windowSize.y - 500.0f}, {500.0f, 500.0f});*/

    auto verticalBoxPlayersEntity = scene->CreateEntity();
    auto verticalBoxPlayersWidget = verticalBoxPlayersEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxPlayersWidget->Init({0.0f, 0.0f}, 15.0f);
    canvasPlayersWidget->AddChild(verticalBoxPlayersWidget);

    //Players Health
    for(int i = 0; i < 2; i++)
    {
        auto canvasHealthPlayerEntity = scene->CreateEntity();
        auto canvasHealthPlayerWidget = canvasHealthPlayerEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasHealthPlayerWidget->Init({0.0f, 0.0f});
        verticalBoxPlayersWidget->AddChild(canvasHealthPlayerWidget);

        auto verticalBoxEntity = scene->CreateEntity();
        auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
        verticalBoxWidget->Init({5.0f, 0.0f}, 12.0f);
        verticalBoxWidget->SetSize({96.0f, 60.0f});

        {
            auto textNameEntity = scene->CreateEntity();
            auto textNameWidget = textNameEntity.AddComponent<Engine::UI::TextWidget>();
            textNameWidget->Init("Name", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::Blue);
            verticalBoxWidget->AddChild(textNameWidget);

            auto progressBarHealthEntity = scene->CreateEntity();
            auto progressBarHealthWidget = progressBarHealthEntity.AddComponent<Game::UI::HealthBar>();
            progressBarHealthWidget->Init(
                    {0.0f, 0.0f}, {128.0f, 48.0f}//{96.0f, 36.0f}
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Fillbar_01_Block.png")
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Filler_02.png"));
            verticalBoxWidget->AddChild(progressBarHealthWidget);
        }

        auto background = scene->CreateEntity();
        auto backgroundWidget = background.AddComponent<Engine::UI::SpriteWidget>();
        backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_02_Standard.png"), {0.0f, 0.0f});
        canvasHealthPlayerWidget->AddChild(backgroundWidget);

        backgroundWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});
        canvasHealthPlayerWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});

        canvasHealthPlayerWidget->AddChild(verticalBoxWidget);
    }

    //Timer
    {
        auto canvasTimerEntity = scene->CreateEntity();
        auto canvasTimerWidget = canvasTimerEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasTimerWidget->Init({windowSize.x - 200.0f, 0.0f}, {200.0f, 200.0f});
        verticalBoxPlayersWidget->AddChild(canvasTimerWidget);

        auto verticalBoxEntity = scene->CreateEntity();
        auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetHorizontalBox>();
        verticalBoxWidget->Init({5.0f, 0.0f}, 5.0f);
        canvasTimerWidget->AddChild(verticalBoxWidget);

        auto textTimerEntity = scene->CreateEntity();
        auto textTimerWidget = textTimerEntity.AddComponent<Engine::UI::TextWidget>();
        textTimerWidget->Init("Timer", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::Blue);
        verticalBoxWidget->AddChild(textTimerWidget);

        auto textTimerValueEntity = scene->CreateEntity();
        auto textTimerValueWidget = textTimerValueEntity.AddComponent<Engine::UI::TextWidget>();
        textTimerValueWidget->Init("0", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::Blue);
        verticalBoxWidget->AddChild(textTimerValueWidget);

        auto background = scene->CreateEntity();
        auto backgroundWidget = background.AddComponent<Engine::UI::SpriteWidget>();
        backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_02_Standard.png"), {0.0f, 0.0f});
        canvasTimerWidget->AddChild(backgroundWidget);

        backgroundWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
        canvasTimerWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
    }
}

void GameLayer::OnDetach() {

}

void GameLayer::OnUpdate(Engine::Timestep ts) {

}

void GameLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
