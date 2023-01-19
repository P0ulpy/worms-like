set(ENGINE_HEADERS
        Engine/Core/Application/EngineApplication.hpp
        Engine/ECS/Entity/Entity.hpp
        Engine/ECS/Entity/Entity.tpp
        Engine/ECS/Component/Component.hpp
        Engine/Core/Scene/Scene.hpp
        Engine/Core/UUID.hpp
        Engine/Engine.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.hpp
        Engine/Core/Application/ApplicationLayer.hpp
        Engine/Core/Time.hpp
        Engine/Core/Application/LayerStack.hpp
        Engine/Core/Logger/Logger.hpp
        Engine/Core/Application/ScenesLayer.hpp
        Engine/Core/Logger/ConsoleColor.hpp
        Engine/Renderer/Renderer.hpp
        Engine/Renderer/AllRenderComponents.hpp
        Engine/Core/RTTI/RTTI.hpp
        Engine/ECS/EntitiesRegistry/IComponentSystem.hpp
        Engine/ECS/EntitiesRegistry/ComponentSystem.hpp
        Engine/AssetLoader/AssetLoader.hpp
        Engine/Core/Inputs/WindowEvents.hpp
        Engine/Core/Components/Transform.hpp
        Engine/Core/Components/SpriteRenderer.hpp
        Engine/ECS/ComponentSystem/ComponentSystem.tpp
        Engine/ECS/ComponentSystem/ComponentSystem.hpp
)

set(ENGINE_SOURCES
        Engine/Core/Application/EngineApplication.cpp
        Engine/ECS/Entity/Entity.cpp
        Engine/ECS/Component/Component.cpp
        Engine/Core/Scene/Scene.cpp
        Engine/Core/UUID.cpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.cpp
        Engine/Core/Application/ApplicationLayer.cpp
        Engine/Core/Logger/Logger.cpp
        Engine/Core/Application/ScenesLayer.cpp
        Engine/Renderer/Renderer.cpp
        Engine/ECS/EntitiesRegistry/ComponentSystem.tpp
        Engine/AssetLoader/AssetLoader.cpp
        Engine/Core/Time.cpp
        Engine/Core/Inputs/WindowEvents.cpp
        Engine/Core/Components/Transform.cpp
        Engine/Core/Components/SpriteRenderer.cpp
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_library(Engine
        STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(Engine
        PUBLIC sfml-system sfml-network sfml-graphics sfml-window)