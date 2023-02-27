//
// Created by Admin on 27/02/2023.
//

#pragma once


#include "Engine/Core/Application/ApplicationLayer.hpp"

class ConfigGameMenuLayer : public Engine::ApplicationLayer {
public:
    ConfigGameMenuLayer()
            : Engine::ApplicationLayer("ConfigGameMenu")
    {}

    ~ConfigGameMenuLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

};
