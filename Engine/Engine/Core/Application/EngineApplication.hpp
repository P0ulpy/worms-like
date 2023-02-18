#ifndef PATHFINDER_ENGINEAPPLICATION_HPP
#define PATHFINDER_ENGINEAPPLICATION_HPP

#include <vector>
#include <memory>
#include "ApplicationLayer.hpp"
#include "ScenesLayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Engine
{
    class EngineApplication
    {
    public:
        static EngineApplication* s_Instance;
        static EngineApplication* Get();

    public:
        EngineApplication();
        ~EngineApplication();

        void Init();
        void Run();
        void Stop();

        void PushLayer(ApplicationLayer* layer);
        void RemoveLayer(ApplicationLayer* layer);

        inline sf::RenderWindow& GetWindow() { return m_window; }
        inline bool IsFullscreen() const { return isFullscreen; }
        inline void SetFullscreen(bool fullscreen) { isFullscreen = fullscreen; }
        inline ScenesLayer& GetScenesLayer() { return m_scenesLayer; }

    private:
        sf::RenderWindow m_window;
        sf::Cursor m_cursor;
        bool isFullscreen = false;

        bool m_running = true;
        Timestep m_LastFrameTime = 0;

        // TODO : use a custom stack where we can iterate through
        std::vector<ApplicationLayer*> m_layers {};
        ScenesLayer m_scenesLayer { "Scenes Layer" };
    };
}

#endif //PATHFINDER_ENGINEAPPLICATION_HPP