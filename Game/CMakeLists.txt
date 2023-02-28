set(GAME_HEADERS
        src/main.hpp
        Game/MainMenu.hpp
        Game/Scenes/MainMenuScene.hpp

        Game/GameSystems/GameStatesManager.hpp
        Game/GameSystems/GameManagerPrefab.hpp

        Game/Player/PlayerPrefab.hpp
        Game/Player/PlayerController.hpp
        Game/Player/Player.hpp
)

set(GAME_SOURCES
        src/main.cpp
        Game/MainMenu.cpp

        Game/GameSystems/GameStatesManager.cpp

        Game/Player/PlayerController.cpp
        Game/Player/Player.cpp
)


set(GAME_TARGET_NAME Game)

add_executable(${GAME_TARGET_NAME} ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(${GAME_TARGET_NAME} Engine)


set(CMAKE_SOURCE_ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(CMAKE_RUNTIME_ASSETS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Assets)

add_custom_command(
        TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_ASSETS_DIRECTORY}
        ${CMAKE_RUNTIME_ASSETS_DIRECTORY}
)