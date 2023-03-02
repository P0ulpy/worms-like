//
// Created by Flo on 28/02/2023.
//

#include "Player.hpp"

/*void Player::OnAwake()
{
    m_playerController = GetEntity().GetComponent<PlayerController>();
}*/

void Player::Init(const std::string_view &name) {
    m_name = name;
    m_playerController->SetActive(false);
}

void Player::OnPlayerTurnBegin()
{
    m_playerController->SetActive(true);
}

void Player::OnUpdate(Engine::Timestep dt)
{

}

void Player::OnPlayerTurnEnd()
{
    m_playerController->SetActive(false);
}

void Player::OnTakeDamage(float damage) {
    m_health -= damage;
    if(m_health <= 0.f)
    {
        OnDeath();
    }
}

void Player::OnDeath() {

}

