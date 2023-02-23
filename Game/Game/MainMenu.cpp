//
// Created by Admin on 14/02/2023.
//

#include "MainMenu.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "UI/Layout/Canvas.hpp"
#include "UI/Button/TextButton.hpp"
#include "UI/Button/SpriteButton.hpp"
#include "UI/Layout/HorizontalBox.hpp"
#include "UI/Layout/VerticalBox.hpp"
#include "UI/Layout/Grid.hpp"

#include "Engine/Core/Inputs/InputSignal.h"
#include "UI/Other/ProgressBar.hpp"
#include "UI/Classique/Text.hpp"
#include "UI/Classique/Sprite.hpp"
#include "UI/Button/ItemButton.hpp"

void MainMenuLayer::OnAttach() {

    auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();
    {
    mainCanvas = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Canvas>();
    mainCanvas->Init();

    inventoryCanvas = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Canvas>();
    inventoryCanvas->Init({ static_cast<float>(windowSize.x) - 400, static_cast<float>(windowSize.y) - 400}, { 200, 200 });
    inventoryButtonConnection = InputSignal::GetInstance()->connectScoped(EventType::OpenInventory, [inventoryCanvas = inventoryCanvas]() {
        inventoryCanvas->SetVisibled(!inventoryCanvas->IsVisibled());
    });

    /*auto horizontalBox = new HorizontalBox();
    horizontalBox->SetPosition({ 50, 50 });
    horizontalBox->SetSize({ 200, 100 });*/

    auto verticalBox = new VerticalBox();
    verticalBox->SetPosition({ 150, 100 });
    verticalBox->SetSize({ 200, 100 });
    verticalBox->SetSpacing(10);

    auto buttonPlay = new TextButton();
    buttonPlay->Init("Play", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 50 }, { 200, 100 }, 20.0f, 38);
    buttonPlay->SetOnClick([]() {
        //Engine::EngineApplication::Get()->Stop();
    });

    auto buttonOption = new TextButton();
    buttonOption->Init("Option", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 150 }, { 200, 100 }, 20.0f, 38);
    buttonOption->SetOnClick([]() {
        //Engine::EngineApplication::Get()->Stop();
    });

    auto buttonQuit = new TextButton();
    buttonQuit->Init("Quit", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 250 }, { 200, 100 }, 20.0f, 38);
    buttonQuit->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto buttonTest = new SpriteButton();
    buttonTest->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 });
    buttonTest->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto buttonTest2 = new SpriteButton();
    buttonTest2->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 });
    buttonTest2->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto buttonTest3 = new SpriteButton();
    buttonTest3->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 });
    buttonTest3->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto buttonTest4 = new SpriteButton();
    buttonTest4->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 });
    buttonTest4->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto buttonTest5 = new SpriteButton();
    buttonTest5->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 });
    buttonTest5->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });

    auto progressBar = new ProgressBar();
    progressBar->Init({ 50, 50 }, { 150, 20 }, 10.0f, sf::Color::Green, sf::Color::Red, 1.0f, sf::Color::Blue);

    auto text = new Text();
    text->Init("Test", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 350, 550 }, 80.0f, sf::Color::Red);

    auto sprite = new Sprite();
    sprite->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 650, 50 });
    sprite->SetScale({1.5f, 1.5f});

    auto grid = new Grid();
    grid->SetPosition({ 0,0});
    grid->SetSize({ 200, 100 });
    grid->SetSpacing(2);
    grid->SetGridSize(3, 2);

    grid->AddWidget(buttonTest);
    grid->AddWidget(buttonTest2);
    grid->AddWidget(buttonTest3);
    grid->AddWidget(buttonTest4);
    grid->AddWidget(buttonTest5);


    /*horizontalBox->AddWidget(buttonPlay);
    horizontalBox->AddWidget(buttonOption);
    horizontalBox->AddWidget(buttonQuit);

    canvas->AddWidget(horizontalBox);*/

    verticalBox->AddWidget(buttonPlay);
    verticalBox->AddWidget(buttonOption);
    verticalBox->AddWidget(buttonQuit);

    mainCanvas->AddWidget(verticalBox);
    mainCanvas->AddWidget(progressBar);
    mainCanvas->AddWidget(text);
    mainCanvas->AddWidget(sprite);

    inventoryCanvas->AddWidget(grid);

    auto itemButton = new ItemButton();
    itemButton->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/hud_coins.png"), { 50, 350 }, ItemSide::BottomRight);

    auto text2 = new Text();
    text2->Init("1", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 350, 550 }, 18.0f, sf::Color::Blue);

    itemButton->AddWidget(text2);

    mainCanvas->AddWidget(itemButton);
    }

    mainCanvas = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Canvas>();
    mainCanvas->Init({ 0, 0 }, { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) });

    {
        auto buttonPlay = new TextButton();
        buttonPlay->Init("Play", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 0, 50 }, { 300, 150 }, 20.0f, 45);
        buttonPlay->SetOnClick([]() {
            //Engine::EngineApplication::Get()->Stop();
        });

        auto buttonOption = new TextButton();
        buttonOption->Init("Option", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 0, 150 }, { 300, 150 }, 20.0f, 45);
        buttonOption->SetOnClick([]() {
            //Engine::EngineApplication::Get()->Stop();
        });

        auto buttonQuit = new TextButton();
        buttonQuit->Init("Quit", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 0, 250 }, { 300, 150 }, 20.0f, 45);
        buttonQuit->SetOnClick([]() {
            Engine::EngineApplication::Get()->Stop();
        });

        auto verticalBox = new VerticalBox();
        verticalBox->SetPosition({ 100,300 });
        verticalBox->SetSize({ 200, 300 });
        verticalBox->SetSpacing(20);

        verticalBox->AddWidget(buttonPlay);
        verticalBox->AddWidget(buttonOption);
        verticalBox->AddWidget(buttonQuit);

        mainCanvas->AddWidget(verticalBox);
    }

    auto title = new Text();
    title->Init("Worms", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { static_cast<float>(windowSize.x / 2), 100 }, 80.0f, sf::Color::Red);
    title->SetPosition({ title->GetPosition().x - title->GetSize().x / 2, title->GetPosition().y - title->GetSize().y / 2 });

    auto sprite = new Sprite();
    sprite->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/p1_stand.png"), { 750, 550 });
    sprite->AddRotation(-10.0f);

    auto sprite2 = new Sprite();
    sprite2->Init(Engine::AssetLoader<sf::Texture>::StaticLoadAsset("../../Assets/p3_stand.png"), { 1050, 800 });
    sprite2->AddRotation(25.0f);


    mainCanvas->AddWidget(title);
    mainCanvas->AddWidget(sprite);
    mainCanvas->AddWidget(sprite2);
}

void MainMenuLayer::OnDetach() {

}

void MainMenuLayer::OnUpdate(Engine::Timestep ts) {

}

void MainMenuLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
