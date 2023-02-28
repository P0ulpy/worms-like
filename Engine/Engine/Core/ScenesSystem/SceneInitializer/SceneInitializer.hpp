//
// Created by Flo on 27/02/2023.
//

#pragma once

#include "../../RTTI/ClassType.hpp"

namespace Engine
{
    class Scene;

    class SceneInitializer
    {
    public:
        virtual void OnLoaded(Scene* scene) = 0;
        virtual void OnUnloaded(Scene* scene) = 0;
    };

} // Engine
