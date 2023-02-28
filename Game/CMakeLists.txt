set(GAME_HEADERS
        src/main.hpp
        Game/MainMenu.hpp
        Game/Prefabs/TestPrefab.hpp)

set(GAME_SOURCES
        src/main.cpp
        Game/MainMenu.cpp
        Game/OptionMenu.cpp Game/OptionMenu.hpp Game/ConfigGameMenu.cpp Game/ConfigGameMenu.hpp Game/Game.cpp Game/Game.hpp ../Engine/Engine/UI/Components/ProgressBar/ProgressBarWidget.cpp ../Engine/Engine/UI/Components/ProgressBar/ProgressBarWidget.hpp UI/Components/ProgressBar/HealthBar.cpp UI/Components/ProgressBar/HealthBar.hpp)


# file(COPY Assets DESTINATION CMAKE_RUNTIME_ASSETS_DIRECTORY)

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