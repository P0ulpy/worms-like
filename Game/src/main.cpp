#pragma once

#include <Engine/Engine.hpp>
#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.hpp"
#include "../Game/MainMenu.hpp"
#include "../Game/OptionMenu.hpp"

#include <Engine/AssetLoader/AssetLoader.hpp>
#include <Engine/Core/Components/Transform.hpp>
#include <Engine/Core/Components/SpriteRenderer.hpp>
#include <Engine/Core/Application/EngineApplication.hpp>


#if defined(SFML_SYSTEM_WINDOWS)
#include <Windows.h>
#endif

using App = Engine::EngineApplication;

/*
class MyComponent : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(MyComponent, Engine::Component)

    void OnAwake() {}
    void OnStart() {}
    void OnUpdate(const float& deltaTime) {}
    void OnImGuiRender() {}
    void OnDestroy() {}
};
*/

int main(int argc, char* argv[])
{
    App app;
    //MainMenuLayer;
    OptionMenuLayer optionMenuLayer;

    SignalSystem::InputConfig::Get()->LoadConfig("Config/GameConfig.ini");

    app.GetWindow().create(sf::VideoMode(1920, 1080), "Worms-Like", sf::Style::Default);

#if defined(SFML_SYSTEM_WINDOWS)
    ShowWindow(app.GetWindow().getSystemHandle(), SW_MAXIMIZE);
#endif

    app.GetWindow().setFramerateLimit(60);
    //app.PushLayer(&mainMenuLayer);
    app.PushLayer(&optionMenuLayer);
    app.Init();

    SignalSystem::InputSignal::Get()->connect("close_window", [&app = app](){ std::cout << "Close" << std::endl; });
    SignalSystem::InputSignal::Get()->connect("pause", [&app = app](){ std::cout << "Pause" << std::endl; });
    SignalSystem::InputSignal::Get()->connect("resized", [&app = app](){ std::cout << "Resized" << std::endl; });*/

    app.Run();
}