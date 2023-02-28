//
// Created by Admin on 14/02/2023.
//

#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/Core/ScenesSystem/ScenesSystem.hpp"

#include "MainMenu.hpp"
#include "Scenes/MainMenuScene.hpp"
#include "Scenes/GameScene.hpp"

void GameLayer::OnAttach()
{
    Engine::ScenesSystem::Get()->DeclareSceneInitializer<MainMenuSceneInitializer>("MainMenu");
    Engine::ScenesSystem::Get()->DeclareSceneInitializer<GameSceneInitializer>("Game");

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
