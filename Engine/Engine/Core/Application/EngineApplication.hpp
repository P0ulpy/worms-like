#pragma once

#include <vector>
#include <memory>
#include "ApplicationLayer.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

namespace Engine
{
    class ScenesLayer;

    class EngineApplication
    {
    public:
        [[nodiscard]] static EngineApplication* Get();

    private:
        static EngineApplication* s_Instance;
<<<<<<< HEAD
=======
        static EngineApplication* Get();
        static constexpr float FixedTimeMs = 200.f;
>>>>>>> Update physics and collisions

    public:
        explicit EngineApplication(sf::VideoMode VideoMode = sf::VideoMode(500, 500));
        ~EngineApplication();

        void Init();
        void Run();
        void Stop();

        void PushLayer(ApplicationLayer* layer);
        void RemoveLayer(ApplicationLayer* layer);

        inline sf::RenderWindow& GetWindow() { return m_window; }
        inline ScenesLayer* GetScenesLayer() { return m_scenesLayer.get(); }

    private:
        sf::RenderWindow m_window;

        bool m_initialized = false;
        bool m_running = false;
        Timestep m_LastFrameTime = 0;

        // TODO : use a custom stack where we can iterate through
        std::vector<ApplicationLayer*> m_layers {};
        std::unique_ptr<ScenesLayer> m_scenesLayer { nullptr };
    };
}