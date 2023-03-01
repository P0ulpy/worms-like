set(GAME_HEADERS
        src/main.hpp
        Game/MainMenu.hpp
        Game/Prefabs/TestPrefab.hpp
        Game/Scenes/MainMenuScene.hpp

        src/Map/Map.hpp src/Modules/PerlinNoise.hpp

        Game/Controllers/SidePlayerController.hpp

        Game/Actors/PlayerCharacter.hpp
)

set(GAME_SOURCES
        src/main.cpp

        Game/MainMenu.cpp

        src/Map/Map.cpp
        src/Modules/PerlinNoise.cpp

        Game/Controllers/SidePlayerController.cpp

        Game/Actors/PlayerCharacter.cpp
)
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