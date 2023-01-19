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
        EngineApplication();
        ~EngineApplication();

        void Run();
        void Stop();

        void PushLayer(ApplicationLayer* layer);
        void RemoveLayer(ApplicationLayer* layer);

        inline ScenesLayer& GetScenesLayer() { return m_scenesLayer; }

    private:
        sf::RenderWindow m_window;

        bool m_running = true;
        Timestep m_LastFrameTime = 0;

        // TODO : use a custom stack where we can iterate throw
        std::vector<ApplicationLayer*> m_layers { nullptr };
        ScenesLayer m_scenesLayer { m_window };
    };
}

#endif //PATHFINDER_ENGINEAPPLICATION_HPP