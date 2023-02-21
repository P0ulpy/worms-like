#include <Engine/Engine.hpp>
#include <SFML/Graphics.hpp>

#include "Engine/AssetLoader/AssetLoader.hpp"
#include "Engine/Core/Components/Transform.hpp"
#include "Engine/Core/Components/SpriteRenderer.hpp"
#include "Engine/ECS/Entity/Entity.hpp"

#include "Engine/Core/Inputs/InputConfig.hpp"
#include "Engine/Core/Inputs/InputSignal.hpp"

/*#include <iostream>*/

class App : public Engine::EngineApplication
{

};

App app;

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

class GameLayer : public Engine::ApplicationLayer
{
public:
    GameLayer()
            : Engine::ApplicationLayer("GameLayer")
    {}

    void OnAttach() override
    {
        // Testing purposes

        auto& sceneLayer = app.GetScenesLayer();
        auto* scene = sceneLayer.GetActiveScene();

        auto krab = scene->CreateEntity();

        auto* transform = krab.AddComponent<Engine::Transform>();
        auto* spriteRenderer = krab.AddComponent<Engine::SpriteRenderer>();

        spriteRenderer->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("./Assets/krab.png"));

        transform->Pos = { 100.f, 100.f };
    }

    void OnDetach() override
    {

    }

    void OnUpdate(Engine::Timestep ts) override {  };
    void OnImGuiRender() override { };
};

GameLayer gameLayer;

int main(int argc, char* argv[])
{
    app.PushLayer(&gameLayer);
    app.Init();

/*    InputConfig::Get()->LoadConfig("../../Config/GameConfig.ini");
    InputSignal::Get()->connect("close_window", [&app = app](){ std::cout << "Close" << std::endl; });
    InputSignal::Get()->connect("pause", [&app = app](){ std::cout << "Pause" << std::endl; });
    InputSignal::Get()->connect("resized", [&app = app](){ std::cout << "Resized" << std::endl; });*/

    app.Run();
}