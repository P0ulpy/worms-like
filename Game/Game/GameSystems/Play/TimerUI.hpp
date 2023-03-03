//
// Created by Flo on 02/03/2023.
//

#include "../../../../Engine/Engine/Core/ScenesSystem/ScenesSystem.hpp"
#include "../../../../Engine/Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "../../../../Engine/Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "../../../../Engine/Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "TeamsTimersUI.hpp"

class TimerUI
{
public:
    TimerUI() = delete;
    explicit TimerUI(Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget)
    {
        m_verticalBoxPlayersWidget = verticalBoxPlayersWidget;

        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
        auto windowSize = sf::Vector2f { Engine::EngineApplication::Get()->GetWindow().getSize() };

        planTimerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
        planTimerWidget->Init({windowSize.x - 200.0f, 0.0f}, {200.0f, 200.0f});
        verticalBoxPlayersWidget->AddChild(planTimerWidget, 0);

        horizontalBoxWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetHorizontalBox>();
        horizontalBoxWidget->Init({5.0f, 5.0f}, 20.0f);
        planTimerWidget->AddChild(horizontalBoxWidget, 0);

        //Turn Timer

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

        //Global Timer
        teamsTimersUI = std::make_unique<TeamsTimersUI>(verticalBoxPlayersWidget);

        backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
        backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
        planTimerWidget->AddChild(backgroundWidget, -1);

        backgroundWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
        planTimerWidget->SetSize(horizontalBoxWidget->GetSize() + sf::Vector2f{10.0f, 10.0f});
    }

    ~TimerUI()
    {
        auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();
        scene->DestroyEntity(planTimerWidget->GetEntity());
        scene->DestroyEntity(horizontalBoxWidget->GetEntity());
        scene->DestroyEntity(planTimerTurnWidget->GetEntity());
        scene->DestroyEntity(textTimerTurnValueWidget->GetEntity());
        scene->DestroyEntity(backgroundTimerTurnValueWidget->GetEntity());
        scene->DestroyEntity(backgroundWidget->GetEntity());
    }

    Engine::UI::WidgetPlan* planTimerWidget { nullptr };
    Engine::UI::WidgetHorizontalBox* horizontalBoxWidget { nullptr };
    Engine::UI::WidgetPlan* planTimerTurnWidget { nullptr };
    Engine::UI::TextWidget* textTimerTurnValueWidget  { nullptr };
    Engine::UI::SpriteWidget* backgroundTimerTurnValueWidget  { nullptr };
    Engine::UI::SpriteWidget* backgroundWidget { nullptr };

    Engine::UI::WidgetVerticalBox* m_verticalBoxPlayersWidget { nullptr };

    std::unique_ptr<TeamsTimersUI> teamsTimersUI { nullptr };
};