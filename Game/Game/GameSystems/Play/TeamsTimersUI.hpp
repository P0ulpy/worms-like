//
// Created by Flo on 02/03/2023.
//

#pragma once

#include "../Team/TeamUI.hpp"

#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "GlobalTimerUI.hpp"

class TeamsTimersUI
{
public:
    TeamsTimersUI() = delete;
    explicit TeamsTimersUI(Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget)
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
        auto windowSize = sf::Vector2f { Engine::EngineApplication::Get()->GetWindow().getSize() };

        planTimerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planTimerWidget->Init({windowSize.x - 200.0f, 0.0f}, {200.0f, 200.0f});
        verticalBoxPlayersWidget->AddChild(planTimerWidget, 0);

        horizontalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetHorizontalBox>();
        horizontalBoxWidget->Init({5.0f, 5.0f}, 20.0f);
        planTimerWidget->AddChild(horizontalBoxWidget, 0);

        //teamUI = std::make_unique<TeamUI>(horizontalBoxWidget);
        globalTimerUI = std::make_unique<GlobalTimerUI>(horizontalBoxWidget);

        backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
        backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
        planTimerWidget->AddChild(backgroundWidget, -1);

        backgroundWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f { 10.0f, 10.0f });
        planTimerWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f { 10.0f, 10.0f });
    }

    ~TeamsTimersUI()
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
        scene->DestroyEntity(planTimerWidget->GetEntity());
        scene->DestroyEntity(horizontalBoxWidget->GetEntity());
        scene->DestroyEntity(backgroundWidget->GetEntity());
    }

    Engine::UI::WidgetPlan* planTimerWidget { nullptr };
    Engine::UI::WidgetHorizontalBox* horizontalBoxWidget { nullptr };
    Engine::UI::SpriteWidget* backgroundWidget { nullptr };

    //std::unique_ptr<TeamUI> teamUI { nullptr };
    std::unique_ptr<GlobalTimerUI> globalTimerUI { nullptr };
};