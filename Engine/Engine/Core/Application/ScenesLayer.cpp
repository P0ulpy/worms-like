//
// Created by Flo on 01/12/2022.
//

#include "ScenesLayer.hpp"
#include "EngineApplication.hpp"

#include <memory>

namespace Engine
{
    ScenesLayer::ScenesLayer(const std::string_view &name)
        : ApplicationLayer::ApplicationLayer(name)
        , _renderTarget(EngineApplication::Get()->GetWindow())
    {}

    void ScenesLayer::OnAttach() {
        m_activeScene = std::make_unique<Scene>();
    }

    void ScenesLayer::OnUpdate(Timestep ts)
    {
        m_activeScene->OnUpdate(ts);
        m_activeScene->RenderScene(_renderTarget);
    }

} // Engine