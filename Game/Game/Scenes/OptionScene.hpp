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
            auto xTitre = (static_cast<float>(windowSize.x) / 2) - (titre->GetSize().x / 2);
            titre->SetPosition({ xTitre, 100.0f });

            canvasWidget->AddChild(titre, 0);

            auto keys = SignalSystem::InputConfig::Get()->GetBindingsKeys();
            auto gridKeysEntity = scene->CreateEntity();
            auto gridKeys = gridKeysEntity.AddComponent<Engine::UI::WidgetGrid>();
            gridKeys->Init({2, static_cast<unsigned int>(keys.size())}, {10, 10}, {150, 350}, {300, 50});
            canvasWidget->AddChild(gridKeys, 0);

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

            auto mouses = SignalSystem::InputConfig::Get()->GetBindingsMouse();
            auto gridMousesEntity = scene->CreateEntity();
            auto gridMouses = gridMousesEntity.AddComponent<Engine::UI::WidgetGrid>();
            gridMouses->Init({2, static_cast<unsigned int>(mouses.size())}, {10, 10}, {static_cast<float>(windowSize.x - 850), 350}, {300, 50});
            canvasWidget->AddChild(gridMouses, 0);

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

            auto backEntity = scene->CreateEntity();
            auto backTextButton = backEntity.AddComponent<Engine::UI::TextButtonWidget>();
            backTextButton->Init("Back", Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0, 0}, 0.0f, 80);
            auto xBack = (static_cast<float>(windowSize.x) / 2) - (backTextButton->GetSize().x / 2);
            auto yBack = static_cast<float>(windowSize.y) - backTextButton->GetSize().y - 100.0f;
            backTextButton->SetPosition({ xBack, yBack });
            backTextButton->SetOnClick([]() {
                Engine::ScenesSystem::Get()->LoadScene("MainMenu");
            });

            auto backgroundEntity = scene->CreateEntity();
            auto background = backgroundEntity.AddComponent<Engine::UI::SpriteWidget>();
            background->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/Background/Background.png"), {0, 0});
            background->SetSize({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

            canvasWidget->AddChild(background, -1);
            canvasWidget->AddChild(backTextButton, 0);
        }

        void OnUnloaded(Engine::Scene* scene) override
        {

        }
};
