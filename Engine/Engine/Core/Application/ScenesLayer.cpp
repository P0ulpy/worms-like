//
// Created by Flo on 01/12/2022.
//

#include "ScenesLayer.hpp"

#include <memory>

namespace Engine
{
    ScenesLayer::ScenesLayer(sf::RenderTarget &renderTarget, const std::string_view &name)
        : ApplicationLayer::ApplicationLayer(name)
        , m_renderTarget(renderTarget)
    {}

    void ScenesLayer::OnAttach() {
        m_activeScene = std::make_unique<Scene>();
    }

    void ScenesLayer::OnUpdate(Timestep ts)
    {
        m_activeScene->OnUpdate(ts);
        m_activeScene->RenderScene(m_renderTarget);
    }

} // Engine