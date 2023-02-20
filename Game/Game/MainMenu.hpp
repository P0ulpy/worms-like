//
// Created by Admin on 14/02/2023.
//

#ifndef POPOSIBENGINE_MAINMENU_HPP
#define POPOSIBENGINE_MAINMENU_HPP


#include "Engine/Core/Application/ApplicationLayer.hpp"
#include "Engine/Core/Inputs/Connection.hpp"

class Canvas;
class TextButton;
class SpriteButton;

class MainMenuLayer : public Engine::ApplicationLayer {

public:
    MainMenuLayer()
            : Engine::ApplicationLayer("MainMenu")
    {}

    ~MainMenuLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

private:
    Canvas* mainCanvas = nullptr;
    Canvas* inventoryCanvas = nullptr;

    ScopedConnection inventoryButtonConnection;
};


#endif //POPOSIBENGINE_MAINMENU_HPP
