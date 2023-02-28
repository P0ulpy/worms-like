//
// Created by Flo on 01/12/2022.
//
#pragma once

#include <memory>

#include "../../Application/ApplicationLayer.hpp"
#include "../ScenesSystem.hpp"

namespace Engine
{
    class ScenesLayer : public ApplicationLayer
    {
    public:
        ScenesLayer() = delete;
        explicit ScenesLayer(sf::RenderTarget* renderTarget, const std::string_view& name = "Layer");

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;

    private:
        ScenesSystem m_scenesSystem { this };
        sf::RenderTarget* m_renderTarget = nullptr;
    };

} // Engine