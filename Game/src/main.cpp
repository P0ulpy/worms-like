/*
#include <Engine/Engine.hpp>

Engine::EngineApplication app;

int main(int argc, char* argv[])
{
    app.Run();
    return 0;
}
*/

#include <Engine/Engine.hpp>

#include <SFML/Graphics.hpp>
#include <array>

class App : public Engine::EngineApplication
{

};

App app;

/*class Transform : public Engine::Component
{
public:
    DECLARE_RTTI(SpriteRenderer, Engine::Component)

    sf::Vector2<float> Pos { .0f, .0f };
    sf::Vector2<float> Scale { 1.f, 1.f };
    float Angle = .0f;

    sf::Transform TransformationMatrix;
};

class SpriteRenderer : public Engine::Component
{
public:
    DECLARE_RTTI(SpriteRenderer, Engine::Component)

    sf::Sprite RenderSprite;
    sf::Texture* Texture = nullptr;
    Transform* transform = nullptr;

    void Init(sf::Texture* texture)
    {
        Texture = texture;
        RenderSprite.setTexture(*Texture);
    }

    void OnStart()
    {
        transform = GetEntity().GetComponent<Transform>();
    }

    void OnRender(sf::RenderTarget &renderTarget) const
    {
        renderTarget.draw(RenderSprite);
    }

    void OnUpdate(const float& deltaTime)
    {
        if(transform)
        {
            RenderSprite.setPosition(transform->Pos);
            RenderSprite.setRotation(transform->Angle);
            RenderSprite.setScale(transform->Scale);
        }
    }
};

class PathFindingManager : public Engine::Component
{
public:
    DECLARE_RTTI(SpriteRenderer, Engine::Component)

    void OnAwake() {}
    void OnStart() {}
    void OnUpdate(const float& deltaTime) {}
    void OnImGuiRender() {}
    void OnDestroy() {}
};

class PathfinderLayer : public Engine::ApplicationLayer
{
public:
    PathfinderLayer()
            : Engine::ApplicationLayer("PathfinderLayer")
    {}

    void OnAttach() override
    {
        // Texting purposes

        auto& sceneLayer = app.GetSceneLayer();
        auto& scene = sceneLayer.GetScene();

        auto pathFindingManager = scene.CreateEntity();
        pathFindingManager.AddComponent<PathFindingManager>();

        auto tile = scene.CreateEntity();
        tile.AddComponent<Transform>();
        auto* spriteRenderer = tile.AddComponent<SpriteRenderer>();

        spriteRenderer->Init(Engine::AssetLoader<sf::Texture>::StaticGetAsset("./Assets/krab.png"));
    }

    void OnDetach() override
    {

    }

    void OnUpdate(Engine::Timestep ts) override {  };
    void OnImGuiRender() override { };
};

PathfinderLayer pathfinderLayer;
*/
int main()
{
    //app.PushLayer(&pathfinderLayer);
    app.Run();
}