//
// Created by Admin on 14/02/2023.
//

#pragma once


#include "Engine/Core/Application/ApplicationLayer.hpp"
//#include "Engine/Core/Inputs/Connection.hpp"

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
    //ScopedConnection connection;
};
