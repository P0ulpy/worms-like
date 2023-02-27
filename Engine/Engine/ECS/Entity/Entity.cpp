//
// Created by Flo on 30/01/2023.
//

#include "Entity.hpp"
#include "../Scene/Scene.hpp"

namespace Engine
{
    Entity::Entity(EntityHandle handle, Scene* scene)
            : m_handle(handle)
            , m_scene(scene)
    {}

} // Engine