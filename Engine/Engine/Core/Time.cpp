//
// Created by Flo on 15/12/2022.
//

#include "Time.hpp"

namespace Engine
{
    Time* Time::Get()
    {
        static Time instance;
        return &instance;
    }

    Timestep Time::RestartDeltaTimeClock()
    {
        s_deltaTime = m_deltaTimeClock.restart().asSeconds();
        return s_deltaTime;
    }
}