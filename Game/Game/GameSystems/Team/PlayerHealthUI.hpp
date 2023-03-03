//
// Created by Flo on 02/03/2023.
//

#pragma once

#include "../../../../Engine/Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "../../../../Engine/Engine/UI/Components/Text/TextWidget.hpp"
#include "../../../../Engine/Engine/AssetLoader/AssetLoader.hpp"
#include "../../../../Engine/Engine/UI/Components/Sprite/SpriteWidget.hpp"

#include "../../../UI/Components/ProgressBar/HealthBar.hpp"

class PlayerHealthUI
{
public:
    PlayerHealthUI() = delete;
    explicit PlayerHealthUI(Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget, const std::string_view& name)
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

        auto planHealthPlayerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planHealthPlayerWidget->Init({0.0f, 0.0f});
        verticalBoxPlayersWidget->AddChild(planHealthPlayerWidget, 0);

        auto verticalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetVerticalBox>();
        verticalBoxWidget->Init({5.0f, 0.0f}, 12.0f);
        verticalBoxWidget->SetSize({96.0f, 60.0f});

        {
            auto textNameWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
            textNameWidget->Init(name.data(), Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::White);
            verticalBoxWidget->AddChild(textNameWidget, 0);

            auto* progressBarHealthWidget = scene->CreateEntity().AddComponent<Game::UI::HealthBar>();
            progressBarHealthWidget->Init(
                    {0.0f, 0.0f}, {128.0f, 48.0f}//{96.0f, 36.0f}
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Fillbar_01_Block.png")
                    , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Filler_03.png"));
            verticalBoxWidget->AddChild(progressBarHealthWidget, 0);
        }

        auto backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
        backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
        planHealthPlayerWidget->AddChild(backgroundWidget, -1);

        backgroundWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});
        planHealthPlayerWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});

        planHealthPlayerWidget->AddChild(verticalBoxWidget, 0);
    }
};
