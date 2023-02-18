//
// Created by Admin on 14/02/2023.
//

#include "MainMenu.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "UI/Button/Button.hpp"

void MainMenuLayer::OnAttach() {
    buttonPlay = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Button>();
    buttonPlay->Init("Play", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 50 }, { 200, 100 }, 38);
    buttonPlay->SetOnClick([]() {
        //Engine::EngineApplication::Get()->Stop();
    });

    buttonOption = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Button>();
    buttonOption->Init("Option", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 150 }, { 200, 100 }, 38);
    buttonOption->SetOnClick([]() {
        //Engine::EngineApplication::Get()->Stop();
    });

    buttonQuit = Engine::EngineApplication::Get()->GetScenesLayer().GetActiveScene()->CreateEntity().AddComponent<Button>();
    buttonQuit->Init("Quit", Engine::AssetLoader<sf::Font>::StaticLoadAsset("../../Assets/Font.otf"), { 50, 250 }, { 200, 100 }, 38);
    buttonQuit->SetOnClick([]() {
        Engine::EngineApplication::Get()->Stop();
    });
}

void MainMenuLayer::OnDetach() {

}

void MainMenuLayer::OnUpdate(Engine::Timestep ts) {

}

void MainMenuLayer::OnImGuiRender() {
    ApplicationLayer::OnImGuiRender();
}
