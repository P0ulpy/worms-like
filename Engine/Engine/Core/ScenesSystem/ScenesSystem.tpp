#pragma once

namespace Engine
{
    template<class TSceneInitializer>
    void ScenesSystem::DeclareSceneInitializer(const std::string_view &name)
    {
        static_assert(std::is_base_of_v<SceneInitializer, TSceneInitializer>, "TSceneInitializer must inherit from SceneInitializer");
        static_assert(std::is_trivially_constructible_v<TSceneInitializer>, "TSceneInitializer must have a trivial constructor");
        static_assert(std::is_trivially_destructible_v<TSceneInitializer>, "TSceneInitializer must have a trivial destructor");

        if(m_scenesInitializers.contains(name))
            Logger::Warn("ScenesSystemLayer::DeclareSceneInitializer() : SceneInitializer ", name, " is already declared ! You are about to override it !");

        m_scenesInitializers[name] = std::make_unique<TSceneInitializer>();
    }
}