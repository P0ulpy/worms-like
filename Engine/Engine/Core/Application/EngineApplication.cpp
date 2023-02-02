#include "EngineApplication.hpp"
#include "../Inputs/WindowEvents.hpp"

Engine::EngineApplication* Engine::EngineApplication::s_Instance = nullptr;

Engine::EngineApplication* Engine::EngineApplication::Get()
{
    return s_Instance;
}

Engine::EngineApplication::EngineApplication()
    : m_window(sf::RenderWindow(
    sf::VideoMode(500, 500),
    "Engine Window",
    sf::Style::Close | sf::Style::Resize))
{
    if(!s_Instance)
        s_Instance = this;
    else
        throw std::runtime_error("EngineApplication already exists");

    PushLayer(&m_scenesLayer);
}

Engine::EngineApplication::~EngineApplication()
{
    RemoveLayer(&m_scenesLayer);
}

void Engine::EngineApplication::PushLayer(Engine::ApplicationLayer *layer)
{
    m_layers.push_back(layer);
    layer->OnAttach();
}

void Engine::EngineApplication::RemoveLayer(Engine::ApplicationLayer *layer)
{
    m_layers.erase(std::find(m_layers.begin(), m_layers.end(),layer));
    layer->OnDetach();
}

void Engine::EngineApplication::Init()
{

}

void Engine::EngineApplication::Run()
{
    m_window.display();

    while (m_window.isOpen() && m_running)
    {
        Timestep time = Time::GetTime();
        Timestep timeStep = time - m_LastFrameTime;

        WindowEvents::ProcessEvents(m_window);

        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnUpdate(timeStep);
        }

        // TODO : Support ImGui
#ifdef IMGUI_SUPPORT
        m_imGuiLayer.Begin();
        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnImGuiRender();
        }
        m_imGuiLayer.End();
#endif
        if(WindowEvents::GetEvent(sf::Event::Closed))
            Stop();
    }
}

void Engine::EngineApplication::Stop()
{
    m_running = false;
    m_window.close();
}
