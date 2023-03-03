//
// Created by Flo on 28/02/2023.
//

#include "Player.hpp"

#include "Engine/UI/Layout/Plan/WidgetPlan.hpp"
#include "Engine/UI/Components/Text/TextWidget.hpp"
#include "Engine/AssetLoader/AssetLoader.hpp"
#include "Engine/UI/Components/Sprite/SpriteWidget.hpp"
#include "Engine/Core/ScenesSystem/ScenesSystem.hpp"

#include "../../UI/Components/ProgressBar/HealthBar.hpp"
#include "../Prefabs/GrenadePrefab.h"
#include "../GameSystems/GameStatesManager.hpp"

void Player::Init(const std::string &name, Engine::UI::WidgetVerticalBox* verticalBoxPlayersWidget)
{
    m_name = name;

    auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

    auto planHealthPlayerWidget = scene->CreateEntity().AddComponent<Engine::UI::WidgetPlan>();
    planHealthPlayerWidget->Init({0.0f, 0.0f});
    verticalBoxPlayersWidget->AddChild(planHealthPlayerWidget, 0);

    auto verticalBoxEntity = scene->CreateEntity();
    auto verticalBoxWidget = verticalBoxEntity.AddComponent<Engine::UI::WidgetVerticalBox>();
    verticalBoxWidget->Init({5.0f, 0.0f}, 12.0f);
    verticalBoxWidget->SetSize({96.0f, 60.0f});

    {
        auto textNameWidget = scene->CreateEntity().AddComponent<Engine::UI::TextWidget>();
        textNameWidget->Init(name.data(), Engine::AssetLoader<sf::Font>::StaticGetAsset("Assets/Font/Font.otf"), {0.0f, 0.0f}, 22, sf::Color::White);
        verticalBoxWidget->AddChild(textNameWidget, 0);

        progressBarHealthWidget = scene->CreateEntity().AddComponent<Game::UI::HealthBar>();
        progressBarHealthWidget->Init(
                {0.0f, 0.0f}, {128.0f, 48.0f}//{96.0f, 36.0f}
                , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Fillbar_01_Block.png")
                , *Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Filler_03.png"));
        verticalBoxWidget->AddChild(progressBarHealthWidget, 0);
    }

    auto backgroundWidget = scene->CreateEntity().AddComponent<Engine::UI::SpriteWidget>();
    backgroundWidget->Init(*Engine::AssetLoader<sf::Texture>::StaticGetAsset("Assets/GUI/Sprites/UI_Flat_Frame_03_Standard.png"), {0.0f, 0.0f});
    planHealthPlayerWidget->AddChild(backgroundWidget, -1);

    backgroundWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});
    planHealthPlayerWidget->SetSize(verticalBoxWidget->GetSize() + sf::Vector2f{10.0f, 5.0f});

    planHealthPlayerWidget->AddChild(verticalBoxWidget, 0);
}

void Player::OnPlayerTurnBegin()
{
    Engine::Logger::Log("Player ", std::string(m_name), " turn begin");

    m_playerCharacter->StateMachine->UnlockMouvement();

    auto* scene = Engine::ScenesSystem::Get()->GetActiveScene();

    auto* PlayerTransform = GetEntity().GetComponent<Engine::Components::Transform>();

    auto* playerController = GameStatesManager::Get()->GetPlayStates()->GetPlayerController();

    playerController->SetPlayerCharacter(m_playerCharacter);
    playerController->SetWeaponToShoot([this, PlayerTransform, scene]()
     {
         auto prefab = scene->InstantiatePrefab<GrenadePrefab>();
         auto posMouse = Engine::EngineApplication::Get()->GetWindow().mapCoordsToPixel({static_cast<float>(sf::Mouse::getPosition().x), static_cast<float>(sf::Mouse::getPosition().y)});
         auto velocityNormalized = PlayerTransform->Pos.GetVectorTo(Maths::Point2D<double>(static_cast<double>(posMouse.x), static_cast<double>(posMouse.y)));
         velocityNormalized.Normalize();
         auto velocity = velocityNormalized * 10.f;
         prefab.GetComponent<Engine::Components::Transform>()->Pos = PlayerTransform->Pos;
         prefab.GetComponent<Engine::Components::Physics::RigidBody2DdComponent>()->GetRigidBody()->LinearVelocity = velocity;
         prefab.GetComponent<Game::Weapons::Grenade>()->Owner = m_playerCharacter;
     });
}

void Player::OnUpdate(Engine::Timestep dt)
{
    if(Engine::WindowEvents::GetEvent(sf::Event::EventType::MouseButtonPressed))
        OnTakeDamage(5);
}

void Player::OnPlayerTurnEnd()
{
    m_playerCharacter->StateMachine->LockMouvement();
}

void Player::OnTakeDamage(float damage)
{
    m_health -= damage;

    progressBarHealthWidget->SetValue(m_health);

    if(m_health <= 0.f)
    {
        OnDeath();
    }
}

void Player::OnDeath() {

}

