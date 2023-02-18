#include <Engine/Engine.hpp>
#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.h"
#include "../Game/Game.hpp"
#include "../Game/GameState/GameState.hpp"

#if defined(SFML_SYSTEM_WINDOWS)
#include <windows.h>
#endif

using App = Engine::EngineApplication;

/*
class MyComponent : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(MyComponent, Engine::Component)

    void OnAwake() {}
    void OnStart() {}
    void OnUpdate(const float& deltaTime) {}
    void OnImGuiRender() {}
    void OnDestroy() {}
};
*/

int main(int argc, char* argv[])
{
    srand (time(nullptr));

    App app;
    GameLayer gameLayer;

    app.GetWindow().setFramerateLimit(60);

    InputSignal::GetInstance()->connect(EventType::CloseWindow, [&app]()
    {
        if(GameState::GetInstance()->GetState() == EGameState::MainMenu
           || GameState::GetInstance()->GetState() == EGameState::EndGame
           || GameState::GetInstance()->GetState() == EGameState::Pause)
            app.Stop();
    });

    InputSignal::GetInstance()->connect(EventType::FullScreen, [&app]()
    {
        if (app.IsFullscreen())
        {
            app.GetWindow().create(sf::VideoMode(1920, 1080), "Worms", sf::Style::Default);

#if defined(SFML_SYSTEM_WINDOWS)
            ShowWindow(app.GetWindow().getSystemHandle(), SW_MAXIMIZE);
#endif
        }
        else
        {
            app.GetWindow().create(sf::VideoMode(1920, 1080), "Worms", sf::Style::Fullscreen);
        }

        app.SetFullscreen(!app.IsFullscreen());
    });

    InputSignal::GetInstance()->Emit({ sf::Event::EventType::KeyPressed, { sf::Keyboard::F11, false, false, false, false } });

    app.PushLayer(&gameLayer);
    app.Init();
    app.Run();
}