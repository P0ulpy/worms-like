//
// Created by Admin on 27/02/2023.
//

#pragma once

#include "Engine/Core/Application/ApplicationLayer.hpp"

class OptionMenuLayer : public Engine::ApplicationLayer {
public:
    OptionMenuLayer()
            : Engine::ApplicationLayer("OptionMenu")
    {}

    ~OptionMenuLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Engine::Timestep ts) override;
    void OnImGuiRender() override;

};
