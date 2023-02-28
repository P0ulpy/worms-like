#pragma once

#include <Engine/Engine.hpp>
#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.hpp"
#include "../Game/MainMenu.hpp"

#include <Engine/AssetLoader/AssetLoader.hpp>
#include <Engine/Core/Components/Transform.hpp>
#include <Engine/Core/Components/SpriteRenderer.hpp>
#include <Engine/Core/Application/EngineApplication.hpp>


#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
#endif

using App = Engine::EngineApplication;

int main(int argc, char* argv[])
{
    App app;
    MainMenuLayer mainMenuLayer;

    SignalSystem::InputConfig::Get()->LoadConfig("Config/GameConfig.ini");

    app.GetWindow().setFramerateLimit(60);
    app.PushLayer(&mainMenuLayer);
    app.Init();

    app.Run();
}