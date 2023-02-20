//
// Created by Flo on 01/12/2022.
//

#include "ScenesLayer.hpp"
#include "EngineApplication.hpp"

#include <memory>
#include <utility>

namespace Engine
{
    ScenesLayer::ScenesLayer(sf::RenderTarget* renderTarget, const std::string_view &name)
        : ApplicationLayer::ApplicationLayer(name)
        , p_renderTarget(renderTarget)
    {}

    void ScenesLayer::OnAttach()
    {
        m_activeScene = std::make_unique<Scene>();
    }

    void ScenesLayer::OnDetach()
    {

    }

    void ScenesLayer::OnUpdate(Timestep ts)
    {
        m_activeScene->OnUpdate(ts);
        m_activeScene->RenderScene(*p_renderTarget);
    }

} // Engine