set(ENGINE_VENDORS
        Engine/Vendor/mINI/ini.hpp
)

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
        Engine/Core/ScenesSystem/ScenesLayer/ScenesLayer.hpp
        Engine/Core/Application/ApplicationLayer.hpp
        Engine/Core/Components/Transform.hpp
        Engine/Core/Components/SpriteRenderer.hpp
        Engine/Core/Components/CompositeComponent.hpp

        Engine/ECS/Handles/Handles.hpp
        Engine/ECS/Entity/Entity.hpp
        Engine/ECS/Component/Component.hpp
        Engine/ECS/ComponentSystem/ComponentSystem.hpp
        Engine/ECS/ComponentSystem/ComponentSystem.tpp
        Engine/ECS/ComponentSystem/IComponentSystem.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.hpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.tpp
        Engine/ECS/Scene/Scene.hpp

        Engine/AssetLoader/AssetLoader.hpp

        Engine/Core/Inputs/WindowEvents.hpp
        Engine/Core/Inputs/InputSignal.hpp
        Engine/Core/Inputs/InputSignal.tpp
        Engine/Core/Inputs/Connection.hpp
        Engine/Core/Inputs/Connection.tpp
        Engine/Core/Inputs/InputConfig.hpp
        Engine/Core/Inputs/Observable.hpp
        Engine/Core/Inputs/Observable.tpp
        Engine/Core/Inputs/Binding.hpp

        Engine/UI/Widget/Widget.hpp
        Engine/UI/Components/Text/TextWidget.hpp
        Engine/UI/Components/Sprite/SpriteWidget.hpp
        Engine/UI/Components/Buttons/ButtonWidget.hpp
        Engine/UI/Components/Buttons/TextButton/TextButtonWidget.hpp
        Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.hpp
        Engine/UI/Canvas/WidgetCanvas.hpp
        Engine/UI/Layout/Grid/WidgetGrid.hpp
        Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.hpp
        Engine/UI/Layout/VerticalBox/WidgetVerticalBox.hpp
        Engine/UI/Components/ProgressBar/ProgressBarWidget.hpp

        Engine/Core/EntityPrefab/EntityPrefab.hpp
        Engine/Core/ScenesSystem/ScenesSystem.hpp
        Engine/Core/ScenesSystem/ScenesSystem.tpp
        Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.hpp
)

set(ENGINE_SOURCES
        Engine/Core/UUID.cpp
        Engine/Core/Time.cpp
        Engine/Core/Logger/Logger.cpp
        Engine/Core/Application/EngineApplication.cpp
        Engine/Core/Application/ApplicationLayer.cpp
        Engine/Core/ScenesSystem/ScenesLayer/ScenesLayer.cpp
        Engine/Core/Components/Transform.cpp
        Engine/Core/Components/SpriteRenderer.cpp
        Engine/Core/Components/CompositeComponent.cpp

        Engine/ECS/Entity/Entity.cpp
        Engine/ECS/Component/Component.cpp
        Engine/ECS/EntitiesRegistry/EntitiesRegistry.cpp
        Engine/ECS/Scene/Scene.cpp

        Engine/AssetLoader/AssetLoader.cpp

        Engine/Core/Inputs/WindowEvents.cpp
        Engine/Core/Inputs/InputSignal.cpp
        Engine/Core/Inputs/Connection.cpp
        Engine/Core/Inputs/InputConfig.cpp
        Engine/Core/Inputs/Observable.cpp

        Engine/UI/Widget/Widget.cpp
        Engine/UI/Components/Text/TextWidget.cpp
        Engine/UI/Components/Sprite/SpriteWidget.cpp
        Engine/UI/Components/Buttons/ButtonWidget.cpp
        Engine/UI/Components/Buttons/TextButton/TextButtonWidget.cpp
        Engine/UI/Components/Buttons/SpriteButton/SpriteButtonWidget.cpp
        Engine/UI/Canvas/WidgetCanvas.cpp
        Engine/UI/Layout/Grid/WidgetGrid.cpp
        Engine/UI/Layout/HorizontalBox/WidgetHorizontalBox.cpp
        Engine/UI/Layout/VerticalBox/WidgetVerticalBox.cpp
        Engine/UI/Components/ProgressBar/ProgressBarWidget.cpp

        Engine/Core/ScenesSystem/ScenesSystem.cpp
        Engine/Core/ScenesSystem/SceneInitializer/SceneInitializer.cpp
        Engine/UI/Layout/Plan/WidgetPlan.cpp Engine/UI/Layout/Plan/WidgetPlan.hpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_library(Engine
        STATIC ${ENGINE_HEADERS} ${ENGINE_SOURCES} ${ENGINE_VENDORS})

target_link_libraries(Engine
        PUBLIC sfml-system sfml-network sfml-graphics sfml-window)