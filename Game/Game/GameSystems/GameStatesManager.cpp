//
// Created by Flo on 28/02/2023.
//

#include <SFML/System/Vector2.hpp>

#include "GameStatesManager.hpp"
#include "Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "../../UI/Layout/Grid/InventoryPlayer.hpp"
#include "Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.hpp"

void GameStatesManager::OnAwake()
{
    CreateUI();
}

void GameStatesManager::OnUpdate(Engine::Timestep dt)
{

}

void GameStatesManager::CreateUI()
{
    auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
    auto windowSize = sf::Vector2f { Engine::EngineApplication::Get()->GetWindow().getSize() };

    auto canvasGameWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetCanvas>();
    canvasGameWidget->Init({0, 0}, {windowSize.x, windowSize.y});

    auto planPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
    planPlayersWidget->Init({30.f, 30.f}, { 500.0f, 500.0f });
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

    auto verticalBoxPlayersWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxPlayersWidget->Init({0.0f, 0.0f}, 15.0f);
    planPlayersWidget->AddChild(verticalBoxPlayersWidget, 0);

    m_playStates = GetEntity().GetComponent<PlayStates>();
    m_playStates->Init(1, 1, verticalBoxPlayersWidget);

    //Inventory
    {
        auto gridInventoryWidget = scene->CreateEntity().AddComponent<Game::UI::InventoryPlayer>();
        gridInventoryWidget->Init({4, 6}, {2.0f, 2.0f}, {0.0f, 0.0f}
            , Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png")
            , Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_01_Lite.png"));
        gridInventoryWidget->SetCellSize({80.0f, 80.0f});

        planInventoryWidget->AddChild(gridInventoryWidget, 0);

        for(int i = 0; i < 5; i++)
        {
            auto itemWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteButtonWidget>();
            itemWidget->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Texture/Items/bomb.png"), {0.0f, 0.0f}, 7.0f);
            gridInventoryWidget->AddChild(itemWidget, 0);
        }
    }
}
