//
// Created by Flo on 26/11/2022.
//

#include "EntitiesRegistry.hpp"
#include "../../Core/Components/Physics.hpp"
#include "../../Maths/Collisions.hpp"

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
        CreateCollisionsKdTree();

        for(auto& [classType, system] : m_updatableSystems)
        {
            system->DispatchUpdate(deltaTime);
        }
    }

    void EntitiesRegistry::CreateCollisionsKdTree()
    {
        std::vector<EntityHandle> ToBeOrdered = {};
        for (auto& [handle, component] : GetSystem<Engine::Components::Physics::RigidBody2DdComponent>()->components) {
            if (!component.HasCollisions)
            {
                continue;
            }
            ToBeOrdered.push_back(handle);
        }
        PhysicsEntityKdTree.CreateKdTree<2, Maths::Point2D<double>, true>(
            ToBeOrdered,
            [this](EntityHandle handle)->Maths::Point2D<double> { return this->GetSystem<Engine::Components::Transform>()->GetOf(handle)->Pos; }
        );
        PhysicsEntityKdTree.CreateKdTree<2, Maths::Point2D<double>, true>(
            ToBeOrdered,
            [this](EntityHandle handle)->Maths::Point2D<double> { return this->GetSystem<Engine::Components::Transform>()->GetOf(handle)->Pos; }
        );
    }

    std::unordered_map<RTTI::ClassType*, IComponentSystem*> EntitiesRegistry::GetAllRenderableSystems()
    {
        return m_renderableSystems;
    }

    void EntitiesRegistry::DestroyAll()
    {
        m_updatableSystems.clear();
        m_renderableSystems.clear();
        m_componentSystems.clear();
    }

} // Engine