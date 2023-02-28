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

class GameSceneInitializer : public Engine::SceneInitializer
{
public:

    void OnLoaded(Engine::Scene* scene) override
    {
        auto windowSize = sf::Vector2f {Engine::EngineApplication::Get()->GetWindow().getSize()};

        auto canvasPlayersEntity = scene->CreateEntity();
        auto canvasPlayersWidget = canvasPlayersEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasPlayersWidget->Init({30.0f, windowSize.y - 400.0f}, {500.0f, 500.0f});

        auto canvasInventoryEntity = scene->CreateEntity();
        auto canvasInventoryWidget = canvasInventoryEntity.AddComponent<Engine::UI::WidgetCanvas>();
        canvasInventoryWidget->Init({windowSize.x - 400.0f, windowSize.y - 550.0f}, {500.0f, 500.0f});
        canvasInventoryWidget->SetActive(false);
        m_inventoryConnection = SignalSystem::InputSignal::Get()->connectScoped("open_inventory", [canvasInventoryWidget]() {
            if(canvasInventoryWidget->IsActive())
                canvasInventoryWidget->SetActive(false);
            else
                canvasInventoryWidget->SetActive(true);
        });

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
                textNameWidget->Init("Name", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::White);
                verticalBoxWidget->AddChild(textNameWidget);

                auto progressBarHealthEntity = scene->CreateEntity();
                auto progressBarHealthWidget = progressBarHealthEntity.AddComponent<Game::UI::HealthBar>();
                progressBarHealthWidget->Init(
                        {0.0f, 0.0f}, {128.0f, 48.0f}//{96.0f, 36.0f}
                        , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Fillbar_01_Block.png")
                        , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Filler_03.png"));
                verticalBoxWidget->AddChild(progressBarHealthWidget);
            }

            auto background = scene->CreateEntity();
            auto backgroundWidget = background.AddComponent<Engine::UI::SpriteWidget>();
            backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
            canvasHealthPlayerWidget->AddChild(backgroundWidget, -1);

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

            auto horizontalBoxEntity = scene->CreateEntity();
            auto horizontalBoxWidget = horizontalBoxEntity.AddComponent<Engine::UI::WidgetHorizontalBox>();
            horizontalBoxWidget->Init({5.0f, 5.0f}, 20.0f);
            canvasTimerWidget->AddChild(horizontalBoxWidget);

            //Turn Timer
            {
                auto canvasTimerTurnEntity = scene->CreateEntity();
                auto canvasTimerTurnWidget = canvasTimerTurnEntity.AddComponent<Engine::UI::WidgetCanvas>();
                canvasTimerTurnWidget->Init({0.0f, 0.0f}, {100.0f, 100.0f});
                horizontalBoxWidget->AddChild(canvasTimerTurnWidget);

                auto textTimerTurnValueEntity = scene->CreateEntity();
                auto textTimerTurnValueWidget = textTimerTurnValueEntity.AddComponent<Engine::UI::TextWidget>();
                textTimerTurnValueWidget->Init("00", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {5.0f, -5.0f}, 60, sf::Color::White);
                canvasTimerTurnWidget->AddChild(textTimerTurnValueWidget);

                auto backgroundTimerTurnValueEntity = scene->CreateEntity();
                auto backgroundTimerTurnValueWidget = backgroundTimerTurnValueEntity.AddComponent<Engine::UI::SpriteWidget>();
                backgroundTimerTurnValueWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_01_Standard.png"), {0.0f, 0.0f});
                canvasTimerTurnWidget->AddChild(backgroundTimerTurnValueWidget, -1);

                backgroundTimerTurnValueWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
                canvasTimerTurnWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
            }

            //Global Timer
            {
                auto verticalBoxEntity = scene->CreateEntity();
                auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
                verticalBoxWidget->Init({0.0f, 0.0f}, 10.0f);
                horizontalBoxWidget->AddChild(verticalBoxWidget);

                auto textTimerGlobalEntity = scene->CreateEntity();
                auto textTimerGlobalWidget = textTimerGlobalEntity.AddComponent<Engine::UI::TextWidget>();
                textTimerGlobalWidget->Init("00:00:00", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 30, sf::Color::White);
                verticalBoxWidget->AddChild(textTimerGlobalWidget);

                //Wind
                {
                    auto canvasWindEntity = scene->CreateEntity();
                    auto canvasWindWidget = canvasWindEntity.AddComponent<Engine::UI::WidgetCanvas>();
                    canvasWindWidget->Init({0.0f, 0.0f}, {100.0f, 100.0f});
                    verticalBoxWidget->AddChild(canvasWindWidget);

                    auto WindEntity = scene->CreateEntity();
                    auto WindWidget = WindEntity.AddComponent<Engine::UI::TextWidget>();
                    WindWidget->Init("------->", Engine::AssetLoader<sf::Font>::StaticGetAsset("../../Assets/Font/Font.otf"), {0.0f, 0.0f}, 30, sf::Color::White);
                    canvasWindWidget->AddChild(WindWidget);

                    auto backgroundWindEntity = scene->CreateEntity();
                    auto backgroundWindWidget = backgroundWindEntity.AddComponent<Engine::UI::SpriteWidget>();
                    backgroundWindWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_01_Standard.png"), {-5.0f, 10.0f});
                    canvasWindWidget->AddChild(backgroundWindWidget, -1);

                    backgroundWindWidget->SetSize({122.5f, 24.0f});
                    canvasWindWidget->SetSize({122.5f, 24.0f});
                }
            }

            auto background = scene->CreateEntity();
            auto backgroundWidget = background.AddComponent<Engine::UI::SpriteWidget>();
            backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
            canvasTimerWidget->AddChild(backgroundWidget, -1);

            backgroundWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
            canvasTimerWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
        }

        //Inventory
        {
            auto gridInventoryEntity = scene->CreateEntity();
            auto gridInventoryWidget = gridInventoryEntity.AddComponent<Game::UI::InventoryPlayer>();
            gridInventoryWidget->Init({4, 6}, {2.0f, 2.0f}, {0.0f, 0.0f}
            , Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png")
            , Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/GUI/Sprites/UI_Flat_Frame_01_Lite.png"));
            gridInventoryWidget->SetCellSize({80.0f, 80.0f});

            canvasInventoryWidget->AddChild(gridInventoryWidget);

            for(int i = 0; i < 24; i++)
            {
                auto itemEntity = scene->CreateEntity();
                auto itemWidget = itemEntity.AddComponent<Engine::UI::SpriteButtonWidget>();
                itemWidget->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("../../Assets/Texture/Items/bomb.png"), {0.0f, 0.0f}, 7.0f);
                gridInventoryWidget->AddChild(itemWidget);
            }
        }
    }

    void OnUnloaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("aba ça game plus padbol");
    }

private:
    SignalSystem::ScopedConnectionSignal m_inventoryConnection;
};