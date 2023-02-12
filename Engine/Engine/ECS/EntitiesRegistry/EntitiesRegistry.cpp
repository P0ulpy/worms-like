//
// Created by Flo on 26/11/2022.
//

#include "EntitiesRegistry.hpp"

namespace Engine
{
    EntityHandle EntitiesRegistry::CreateEntity()
    {
        return EntityHandle(Core::UUID::CreateNew());
    }

    void EntitiesRegistry::DestroyEntity(EntityHandle handle)
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            if(system->Has(handle))
            {
                system->Remove(handle);
            }
        }
    }

    void EntitiesRegistry::AwakeAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchAwake();
        }
    }

    void EntitiesRegistry::StartAll()
    {
        for(auto& [classType, system] : m_componentSystems)
        {
            system->DispatchStart();
        }
    }

    void EntitiesRegistry::UpdateAllUpdatable(const float& deltaTime)
    {
        for(auto& [classType, system] : m_updatableSystems)
        {
            system->DispatchUpdate(deltaTime);
        }
    }

    void EntitiesRegistry::RenderAllRenderer(sf::RenderTarget& renderTarget)
    {
        for(auto& [classType, system] : m_renderableSystems)
        {
            system->DispatchRender(renderTarget);
        }
    }


} // Engine