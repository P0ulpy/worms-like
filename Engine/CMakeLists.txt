set(ENGINE_HEADERS
        Engine/Engine.hpp

        Engine/Core/UUID.hpp
        Engine/Core/Time.hpp
        Engine/Core/RTTI/ClassType.hpp
        Engine/Core/Logger/Logger.hpp
        Engine/Core/Logger/Logger.tpp
        Engine/Core/Logger/ConsoleColor.hpp
        Engine/Core/Application/EngineApplication.hpp
        Engine/Core/Application/LayerStack.hpp
        Engine/Core/Application/ScenesLayer.hpp
        Engine/Core/Application/ApplicationLayer.hpp
        Engine/Core/Components/Transform.hpp
        Engine/Core/Components/SpriteRenderer.hpp
        Engine/Core/Inputs/WindowEvents.hpp
        Engine/Core/Inputs/InputSignal.h
        Engine/Core/Inputs/Connection.hpp
        Engine/Core/Inputs/InputConfig.hpp

        Engine/ECS/Handles/Handles.hpp
        Engine/ECS/Entity/Entity.hpp
        Engine/ECS/Component/Component.hpp
        Engine/ECS/ComponentSystem/ComponentSystem.hpp
        Engine/ECS/ComponentSystem/ComponentSystem.tpp
        Engine/ECS/ComponentSystem/IComponentSystem.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.tpp
        Engine/ECS/Scene/Scene.hpp

        Engine/Renderer/Renderer.hpp
        Engine/Renderer/AllRenderComponents.hpp

        Engine/AssetLoader/AssetLoader.hpp
)

set(ENGINE_SOURCES
        Engine/Core/UUID.cpp
        Engine/Core/Time.cpp
        Engine/Core/Logger/Logger.cpp
        Engine/Core/Application/EngineApplication.cpp
        Engine/Core/Application/ApplicationLayer.cpp
        Engine/Core/Application/ScenesLayer.cpp
        Engine/Core/Components/Transform.cpp
        Engine/Core/Components/SpriteRenderer.cpp
        Engine/Core/Inputs/WindowEvents.cpp
        Engine/Core/Inputs/InputSignal.cpp
        Engine/Core/Inputs/Connection.cpp
        Engine/Core/Inputs/InputConfig.cpp

        Engine/ECS/Entity/Entity.cpp
        Engine/ECS/Component/Component.cpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.cpp
        Engine/ECS/Scene/Scene.cpp

        Engine/Renderer/Renderer.cpp

        Engine/AssetLoader/AssetLoader.cpp
)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_library(Engine
        STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES})

target_link_libraries(Engine
        PUBLIC sfml-system sfml-network sfml-graphics sfml-window)