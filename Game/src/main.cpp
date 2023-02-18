#include <Engine/Engine.hpp>
#include <SFML/Graphics.hpp>

#include "Engine/Core/Inputs/InputSignal.h"
#include "../Game/MainMenu.hpp"

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
    MainMenuLayer mainMenuLayer;

    app.GetWindow().setFramerateLimit(60);

    app.GetWindow().create(sf::VideoMode(1920, 1080), "PoposibEngine", sf::Style::Default);

    app.PushLayer(&mainMenuLayer);
    app.Init();
    app.Run();
}