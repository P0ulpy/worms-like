#include "EngineApplication.hpp"
#include "../Inputs/WindowEvents.hpp"
#include "../Logger/Logger.hpp"

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
        throw std::runtime_error("An instance of EngineApplication already exists");

    m_scenesLayer = std::make_unique<ScenesLayer>(&m_window, "Scenes Layer");
    PushLayer(m_scenesLayer.get());
}

Engine::EngineApplication::~EngineApplication()
{
    Engine::EngineApplication::s_Instance = nullptr;
}

void Engine::EngineApplication::PushLayer(Engine::ApplicationLayer *layer)
{
    m_layers.push_back(layer);
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
    Logger::SetThreadLabel("main");

    for(auto* layer : m_layers)
        layer->OnAttach();

    //Logger::Log("EngineApplication : Layers attached");

    while (m_window.isOpen())
    {
        if(!m_running)
            continue;

        Timestep timeStep = Time::Get()->RestartDeltaTimeClock();

        WindowEvents::ProcessEvents(m_window);

        m_window.clear(sf::Color::Black);

        for(ApplicationLayer* layer : m_layers)
        {
            layer->OnUpdate(timeStep);
        }

        m_window.display();

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
