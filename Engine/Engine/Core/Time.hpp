//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_TIME_HPP
#define PATHFINDER_TIME_HPP

#include "SFML/System/Clock.hpp"
#include "Base.hpp"

namespace Engine
{
    using Timestep = float;

    class Time
    {
    public:
        static Time* Get();

    public:
        Time() = default;

        Timestep RestartDeltaTimeClock();
        [[nodiscard]] Timestep GetDeltaTime() const { return s_deltaTime; }
    private:
        sf::Clock m_deltaTimeClock {};
        Timestep s_deltaTime = .0f;
    };
}

#endif //PATHFINDER_TIME_HPP
