//
// Created by Admin on 27/02/2023.
//

#include "GameLayer.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Text/TextWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "../UI/Components/ProgressBar/HealthBar.hpp"
#include "Engine/UI/Components/Sprite/SpriteWidget.hpp"
#include "Engine/Core/ScenesSystem/ScenesSystem.hpp"

#include "Scenes/GameScene.hpp"
#include "Scenes/MainMenuScene.hpp"
#include "Scenes/OptionScene.hpp"

void GameLayer::OnAttach()
{
    Engine::ScenesSystem::Get()->DeclareSceneInitializer<MainMenuSceneInitializer>("MainMenu");
    Engine::ScenesSystem::Get()->DeclareSceneInitializer<GameSceneInitializer>("Game");
    Engine::ScenesSystem::Get()->DeclareSceneInitializer<OptionSceneInitializer>("Options");

    Engine::ScenesSystem::Get()->LoadScene("MainMenu");
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Engine::Timestep ts) {

}

void GameLayer::OnImGuiRender()
{

}
