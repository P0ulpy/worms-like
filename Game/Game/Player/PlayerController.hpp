//
// Created by Flo on 28/02/2023.
//

#pragma once

#include <Engine/ECS/Component/Component.hpp>

class PlayerController : public Engine::Component
{
public:
    DECLARE_CLASS_TYPE(PlayerController, Engine::Component)

    // This code is a placeholder to create a contract between the PlayerController and game systems
    // it will be replaced by a real implementation later

    void OnAwake() {};
};
