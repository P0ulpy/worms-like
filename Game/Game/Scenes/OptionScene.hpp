//
// Created by Admin on 28/02/2023.
//

#pragma once

#include "Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp"
#include "Engine/Core/Application/EngineApplication.hpp"
#include "Engine/UI/Canvas/WidgetCanvas.hpp"
#include "Engine/UI/Components/Text/TextWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "Engine/Core/Inputs/InputConfig.hpp"
#include "Engine/UI/Layout/Grid/WidgetGrid.hpp"
#include "Engine/Core/Inputs/WindowEvents.hpp"
#include "Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp"
#include "Engine/ECS/Scene/Scene.hpp"

class OptionSceneInitializer : public Engine::SceneInitializer
{
public:
        void OnLoaded(Engine::Scene* scene) override
        {
            auto windowSize = Engine::EngineApplication::Get()->GetWindow().getSize();

            auto canvasEntity = scene->CreateEntity();
            auto canvasWidget = canvasEntity.AddComponent<Engine::UI::WidgetCanvas>();
            canvasWidget->Init();

            auto titre = canvasEntity.AddComponent<Engine::UI::TextWidget>();
            titre->Init("Option Menu", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 100);

            canvasWidget->AddChild(titre, 0);
            canvasWidget->SetChildOptions(titre, {
                .Anchors = Engine::UI::CanvasAnchors::TOP_CENTER,
                .TopOffset = 20.f,
                .Size = titre->GetSize()
            });

            auto keys = SignalSystem::InputConfig::Get()->GetBindingsKeys();
            auto gridKeysEntity = scene->CreateEntity();
            auto gridKeys = gridKeysEntity.AddComponent<Engine::UI::WidgetGrid>();
            gridKeys->Init({2, static_cast<unsigned int>(keys.size())}, {0.f, 0.f}, {0.f, 0.f}, {300, 50});

            for(auto [key, value] : keys) {
                auto keyEntity = scene->CreateEntity();
                auto keyText = keyEntity.AddComponent<Engine::UI::TextWidget>();
                keyText->Init(key, Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 35);
                gridKeys->AddChild(keyText, 0);

                std::string valueString;
                for( auto [k, t] : SignalSystem::BindingsMapKey)
                    if(t == value) {
                        valueString = k;
                        break;
                    }

                auto valueEntity = scene->CreateEntity();
                auto valueTextButton = valueEntity.AddComponent<Engine::UI::TextButtonWidget>();
                valueTextButton->Init(valueString, Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 35);
                valueTextButton->SetOnClick([valueTextButton, gridKeys, key]() {
                    Engine::WindowEvents::lastEvent.connect([valueTextButton, gridKeys, key](const sf::Event &event) {
                        if(event.type == sf::Event::KeyPressed) {
                            std::string valueString = SignalSystem::GetKeyByValue(event.key.code);
                            if(!valueString.empty())
                            {
                                Engine::WindowEvents::UnlockAfterEvent = key;
                                SignalSystem::InputConfig::Get()->SetKeyBinding(key, event.key.code);
                                valueTextButton->SetText(valueString);
                                gridKeys->UpdatePosition();
                                Engine::WindowEvents::lastEvent.forceDisconnect();
                                SignalSystem::InputSignal::Get()->Lock();

                            }
                        }
                    });
                });
                gridKeys->AddChild(valueTextButton, 0);
            }
            canvasWidget->AddChild(gridKeys, 0);
            canvasWidget->SetChildOptions(gridKeys, {
                .Anchors=Engine::UI::CanvasAnchors::CENTER_LEFT,
                .LeftOffset=200.f,
                .Size=gridKeys->GetSize()
            });

            auto mouses = SignalSystem::InputConfig::Get()->GetBindingsMouse();
            auto gridMousesEntity = scene->CreateEntity();
            auto gridMouses = gridMousesEntity.AddComponent<Engine::UI::WidgetGrid>();
            gridMouses->Init({2, static_cast<unsigned int>(mouses.size())}, {0.f, 0.f}, {0.f, 0.f}, {300, 50});

            for(auto [mouse, value] : mouses) {
                auto mouseEntity = scene->CreateEntity();
                auto mouseText = mouseEntity.AddComponent<Engine::UI::TextWidget>();
                mouseText->Init(mouse, Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 35);
                gridMouses->AddChild(mouseText, 0);

                std::string valueString;
                for( auto [k, t] : SignalSystem::BindingsMapMouse)
                    if(t == value) {
                        valueString = k;
                        break;
                    }

                auto valueEntity = scene->CreateEntity();
                auto valueTextButton = valueEntity.AddComponent<Engine::UI::TextButtonWidget>();
                valueTextButton->Init(valueString, Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 35);
                valueTextButton->SetOnClick([valueTextButton, gridMouses, mouse]() {
                    Engine::WindowEvents::lastEvent.connect([valueTextButton, gridMouses, mouse](const sf::Event &event) {
                        if(event.type == sf::Event::MouseButtonPressed) {
                            std::string valueString = SignalSystem::GetMouseByValue(event.mouseButton.button);
                            if(!valueString.empty())
                            {
                                Engine::WindowEvents::UnlockAfterEvent = mouse;
                                SignalSystem::InputConfig::Get()->SetMouseBinding(mouse, event.mouseButton.button);
                                valueTextButton->SetText(valueString);
                                gridMouses->UpdatePosition();
                                Engine::WindowEvents::lastEvent.forceDisconnect();
                                SignalSystem::InputSignal::Get()->Lock();
                            }
                        }
                    });
                });
                gridMouses->AddChild(valueTextButton, 0);
            }
            canvasWidget->AddChild(gridMouses, 0);
            canvasWidget->SetChildOptions(gridMouses, {
                .Anchors=Engine::UI::CanvasAnchors::CENTER_RIGHT,
                .RightOffset=200.f,
                .Size=gridMouses->GetSize()
            });

            auto backEntity = scene->CreateEntity();
            auto backTextButton = backEntity.AddComponent<Engine::UI::TextButtonWidget>();
            backTextButton->Init("Back", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 80);
            backTextButton->SetOnClick([]() {
                Engine::ScenesSystem::Get()->LoadScene("MainMenu");
            });

            auto backgroundEntity = scene->CreateEntity();
            auto background = backgroundEntity.AddComponent<Engine::UI::SpriteWidget>();
            background->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Background/background.png"), {0, 0});

            canvasWidget->AddChild(background, -1);
            canvasWidget->SetChildOptions(background, {
                .Anchors=Engine::UI::CanvasAnchors::STRETCH
            });

            canvasWidget->AddChild(backTextButton, 0);
            canvasWidget->SetChildOptions(backTextButton, {
                .Anchors=Engine::UI::CanvasAnchors::TOP_LEFT,
                .TopOffset = 20.f,
                .LeftOffset = 20.f,
                .Size=backTextButton->GetSize()
            });
        }

        void OnUnloaded(Engine::Scene* scene) override
        {

        }
};
