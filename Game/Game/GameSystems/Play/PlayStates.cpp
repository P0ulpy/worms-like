//
// Created by Flo on 03/03/2023.
//

#include "PlayStates.hpp"
#include "../../../UI/Layout/Grid/InventoryPlayer.hpp"
#include "Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"

#include <Engine/Core/ScenesSystem/ScenesSystem.hpp>

#include <Engine/UI/Layout/Plan/WidgetPlan.hpp>
#include <Engine/AssetLoader/AssetLoader.hpp>
#include <Engine/UI/Components/Text/TextWidget.hpp>
#include <Engine/UI/Components/Sprite/SpriteWidget.hpp>
#include <Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp>

void PlayStates::CreateUI()
{
    auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
    auto windowSize = sf::Vector2f { Engine::EngineApplication::Get()->GetWindow().getSize() };

    auto canvasGameWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetCanvas>();
    canvasGameWidget->Init({0, 0}, {windowSize.x, windowSize.y});

    auto planPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
    planPlayersWidget->Init({ 30.f, 30.f }, { 500.0f, 500.0f });
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

    verticalBoxPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxPlayersWidget->Init({0.0f, 0.0f}, 15.0f);
    planPlayersWidget->AddChild(verticalBoxPlayersWidget, 0);

    //Players Health


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

            textTimerTurnValueWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
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

            textTimerGlobalWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
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
