//
// Created by Flo on 02/03/2023.
//

#pragma once

#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "../../../../Engine/Engine/UI/Components/Text/TextWidget.hpp"
#include "../../../../Engine/Engine/AssetLoader/AssetLoader.hpp"
#include "../../../../Engine/Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "../../../../Engine/Engine/UI/Components/Sprite/SpriteWidget.hpp"

/*class TeamUI
{
public:
    TeamUI() = delete;
    explicit TeamUI(Engine::UI::WidgetHorizontalBox* horizontalBoxWidget)
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

        planTimerTurnWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planTimerTurnWidget->Init({0.0f, 0.0f}, {100.0f, 100.0f});
        horizontalBoxWidget->AddChild(planTimerTurnWidget, 0);

        textTimerTurnValueWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
        textTimerTurnValueWidget->Init("00", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {5.0f, -5.0f}, 60, sf::Color::White);
        planTimerTurnWidget->AddChild(textTimerTurnValueWidget, 0);

        backgroundTimerTurnValueWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
        backgroundTimerTurnValueWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_01_Standard.png"), {0.0f, 0.0f});
        planTimerTurnWidget->AddChild(backgroundTimerTurnValueWidget, -1);

        backgroundTimerTurnValueWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
        planTimerTurnWidget->SetSize(textTimerTurnValueWidget->GetSize() + sf::Vector2f{10.0f, 25.0f});
    }

    ~TeamUI()
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
        scene->DestroyEntity(planTimerTurnWidget->GetEntity());
        scene->DestroyEntity(textTimerTurnValueWidget->GetEntity());
        scene->DestroyEntity(backgroundTimerTurnValueWidget->GetEntity());
    }

    Engine::UI::WidgetPlan* planTimerTurnWidget                 { nullptr };
    Engine::UI::TextWidget* textTimerTurnValueWidget            { nullptr };
    Engine::UI::SpriteWidget* backgroundTimerTurnValueWidget    { nullptr };
};*/