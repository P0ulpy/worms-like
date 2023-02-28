//
// Created by Flo on 28/02/2023.
//

#pragma once

#include "Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"

#include "../Prefabs/TestPrefab.hpp"

class GameSceneInitializer : public Engine::SceneInitializer
{
public:

    void OnLoaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("ça game ou quoi ?");
    }

    void OnUnloaded(Engine::Scene* scene) override
    {
        Engine::Logger::Log("aba ça game plus padbol");
    }
};