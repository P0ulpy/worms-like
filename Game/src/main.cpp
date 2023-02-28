#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.hpp"
#include "../Game/MainMenu.hpp"
#include "../Game/ConfigGameMenu.hpp"
#include "../Game/Game.hpp"
#include "../Game/OptionMenu.hpp"

#include <Engine/Core/Components/Transform.hpp>
#include <Engine/Core/Application/EngineApplication.hpp>


#if defined(SFML_SYSTEM_WINDOWS)
#include <Windows.h>
#endif

using App = Engine::EngineApplication;

int main(int argc, char* argv[])
{
    App app;
    /*MainMenuLayer mainMenuLayer;*/
    /*OptionMenuLayer optionMenuLayer;*/
    /*ConfigGameMenuLayer configGameMenuLayer;*/
    GameLayer gameLayer;

    SignalSystem::InputConfig::Get()->LoadConfig("../../Config/GameConfig.ini");

    app.GetWindow().create(sf::VideoMode(1920, 1080), "Worms-Like", sf::Style::Default);

#if defined(SFML_SYSTEM_WINDOWS)
    ShowWindow(app.GetWindow().getSystemHandle(), SW_MAXIMIZE);
#endif

    app.GetWindow().setFramerateLimit(60);
    app.PushLayer(&gameLayer);
    app.Init();

    SignalSystem::InputSignal::Get()->connect("close_window", [&app = app](){ app.Stop(); });
    SignalSystem::InputSignal::Get()->connect("pause", [&app = app](){ /*std::cout << "Pause" << std::endl;*/ });
    SignalSystem::InputSignal::Get()->connect("resized", [&app = app](){ /*std::cout << "Resized" << std::endl;*/ });

    app.Run();
}