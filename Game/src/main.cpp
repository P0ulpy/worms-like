#pragma once

#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.hpp"
#include "../Game/MainMenu.hpp"

#include <Engine/AssetLoader/AssetLoader.hpp>
#include <Engine/Core/Application/EngineApplication.hpp>


#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
#endif

using App = Engine::EngineApplication;

int main(int argc, char* argv[])
{
    App app;
    GameLayer mainMenuLayer;

    SignalSystem::InputConfig::Get()->LoadConfig("Config/GameConfig.ini");

    app.GetWindow().setFramerateLimit(60);
    app.PushLayer(&mainMenuLayer);
    app.Init();

    app.Run();
}