//
// Created by Admin on 14/02/2023.
//

#pragma once


#include "Engine/Core/Application/ApplicationLayer.hpp"

class GameLayer : public Engine::ApplicationLayer {

public:
    GameLayer()
            : Engine::ApplicationLayer("Game")
    {}

    ~GameLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

private:
    //ScopedConnection connection;
};
