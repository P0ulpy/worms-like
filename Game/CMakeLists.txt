set(GAME_HEADERS
        src/main.hpp
        Game/Scenes/MainMenuScene.hpp
        Game/Scenes/OptionScene.hpp

        Game/GameSystems/GameStatesManager.hpp
        Game/GameSystems/GameManagerPrefab.hpp

        Game/Player/PlayerPrefab.hpp
        Game/Player/PlayerController.hpp
        Game/Player/Player.hpp
        Game/GameLayer.hpp
        UI/Components/ProgressBar/HealthBar.hpp
        UI/Layout/Grid/InventoryPlayer.hpp

        Game/Map/Map.hpp
        Game/Modules/PerlinNoise.hpp

        Game/Controllers/SidePlayerController.hpp

        Game/Actors/PlayerCharacter.hpp
        Game/AssetsPaths.hpp
        Game/GameSystems/Team/PlayerTeam.hpp
        Game/GameSystems/Play/PlayStates.hpp Game/GameSystems/Team/TeamUI.hpp Game/GameSystems/Play/TeamsTimersUI.hpp Game/GameSystems/Play/GlobalTimerUI.hpp)

set(GAME_SOURCES
        src/main.cpp

        Game/GameSystems/GameStatesManager.cpp

        Game/Player/PlayerController.cpp
        Game/Player/Player.cpp
        Game/GameLayer.cpp
        UI/Components/ProgressBar/HealthBar.cpp
        UI/Layout/Grid/InventoryPlayer.cpp

        Game/Controllers/SidePlayerController.cpp

        Game/Actors/PlayerCharacter.cpp

        Game/Map/Map.cpp
        Game/Modules/PerlinNoise.cpp
        Game/Weapons/IWeapons.h Game/Weapons/Grenade.h Game/Weapons/GrenadeFragmentation.h Game/Prefabs/GrenadePrefab.h Game/Prefabs/GrenadeFragPrefab.h Game/Weapons/Grenade.cpp Game/Weapons/GrenadeFragmentation.cpp)
set(GAME_TARGET_NAME Game)

add_executable(${GAME_TARGET_NAME} ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(${GAME_TARGET_NAME} Engine)


set(CMAKE_SOURCE_ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(CMAKE_RUNTIME_ASSETS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Assets)
set(CMAKE_SOURCE_CONFIG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Config)
set(CMAKE_RUNTIME_CONFIG_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Config)

add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_ASSETS_DIRECTORY}
        ${CMAKE_RUNTIME_ASSETS_DIRECTORY}
)

add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_CONFIG_DIRECTORY}
        ${CMAKE_RUNTIME_CONFIG_DIRECTORY}
)