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
    {}

    void ScenesLayer::OnAttach()
    {
        m_activeScene = std::make_unique<Scene>();
        Scene::SetActiveScene(m_activeScene.get());
    }

    void ScenesLayer::OnDetach()
    {
        Scene::SetActiveScene(nullptr);
    }

    void ScenesLayer::OnUpdate(Timestep ts)
    {
        m_activeScene->OnUpdate(ts);
        m_activeScene->RenderScene(EngineApplication::Get()->GetWindow());
    }

} // Engine