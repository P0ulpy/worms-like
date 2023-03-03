//
// Created by Flo on 02/03/2023.
//

#pragma once

#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "../../../../Engine/Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "../../../../Engine/Engine/UI/Components/Text/TextWidget.hpp"
#include "../../../../Engine/Engine/AssetLoader/AssetLoader.hpp"
#include "../../../../Engine/Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "../../../../Engine/Engine/UI/Components/Sprite/SpriteWidget.hpp"

class GlobalTimerUI
{
public:
    GlobalTimerUI() = delete;
    explicit GlobalTimerUI(Engine::UI::WidgetHorizontalBox* horizontalBoxWidget)
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

        auto verticalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
        verticalBoxWidget->Init({0.0f, 0.0f}, 10.0f);
        horizontalBoxWidget->AddChild(verticalBoxWidget, 0);

        auto textTimerGlobalWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
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
};