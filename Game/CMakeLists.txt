set(GAME_HEADERS
        src/main.hpp
        Game/MainMenu.hpp
        Game/UI/IWidget.hpp
        Game/UI/Button/TextButton.hpp
        Game/UI/Button/SpriteButton.hpp
        Game/UI/Button/ButtonBase.hpp
        Game/UI/WidgetComposite.hpp
        Game/UI/Layout/Canvas.hpp
        Game/UI/Layout/HorizontalBox.hpp
        Game/UI/Layout/VerticalBox.hpp
        Game/UI/Layout/Grid.hpp
        Game/UI/Other/ProgressBar.hpp)

set(GAME_SOURCES
        src/main.cpp
        Game/MainMenu.cpp
        Game/UI/Classique/Text.cpp Game/UI/Classique/Text.hpp Game/UI/Classique/Sprite.cpp Game/UI/Classique/Sprite.hpp Game/UI/Button/ItemButton.hpp Game/OptionMenu.cpp Game/OptionMenu.hpp Game/ConfigGameMenu.cpp Game/ConfigGameMenu.hpp Game/Game.cpp Game/Game.hpp)


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