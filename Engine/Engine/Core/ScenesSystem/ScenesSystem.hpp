//
// Created by Flo on 27/02/2023.
//

#pragma once

#include <string_view>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../ECS/Scene/Scene.hpp"
#include "SceneInitializer/SceneInitializer.hpp"

namespace Engine
{
    class ScenesLayer;

    struct WaitingForSceneLoad
    {
        bool waiting = false;
        std::string_view name {};
    };

    class ScenesSystem
    {
    public:
        [[nodiscard]] static ScenesSystem* Get();

    private:
        static ScenesSystem* s_Instance;

    public:
        ScenesSystem() = delete;
        explicit ScenesSystem(ScenesLayer* scenesLayer);
        ~ScenesSystem();

        template <class TSceneInitializer>
        void DeclareSceneInitializer(const std::string_view& name);

        void LoadScene(const std::string_view& name);

        [[nodiscard]] bool IsWaitingForSceneLoad() const { return m_waitingForSceneLoad.waiting; }

        [[nodiscard]] ScenesLayer* GetScenesLayer() const { return m_scenesLayer; }
        [[nodiscard]] Scene* GetActiveScene() const { return m_activeScene.get(); }

    private:
        void ApplyLoadScene();
        void UnloadActiveScene();

    private:
        WaitingForSceneLoad m_waitingForSceneLoad {};

        std::unordered_map<std::string_view, std::unique_ptr<SceneInitializer>> m_scenesInitializers {};
        std::unique_ptr<Scene> m_activeScene { nullptr };
        std::string_view m_activeSceneName {};

        ScenesLayer* m_scenesLayer = nullptr;

        friend class EngineApplication;
    };

    ScenesSystem* ScenesSystem::s_Instance = nullptr;

} // Engine

#include "ScenesSystem.tpp"