//
// Created by Flo on 26/11/2022.
//

#ifndef PATHFINDER_TIME_HPP
#define PATHFINDER_TIME_HPP

#include "Base.hpp"

namespace Engine
{
    // TODO : wright Timestep class
    using Timestep = float;

    struct Time
    {
        static Timestep GetTime();
    };
}

#endif //PATHFINDER_TIME_HPP
