//
// Created by Flo on 01/12/2022.
//

#ifndef PATHFINDER_SCENESLAYER_HPP
#define PATHFINDER_SCENESLAYER_HPP

#include <memory>
#include "SFML/Graphics/RenderTarget.hpp"

#include "ApplicationLayer.hpp"
#include "../../ECS/Scene/Scene.hpp"

namespace Engine
{
    class ScenesLayer : public ApplicationLayer
    {
    public:
        ScenesLayer() = delete;
        explicit ScenesLayer(sf::RenderTarget* renderTarget, const std::string_view& name = "Layer");

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;

        [[nodiscard]] Scene* GetActiveScene() const { return m_activeScene.get(); }

    private:
        std::unique_ptr<Scene> m_activeScene { nullptr };
        sf::RenderTarget* p_renderTarget { nullptr };
    };

} // Engine

#endif //PATHFINDER_SCENESLAYER_HPP
