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
        Game/UI/Classique/Text.cpp Game/UI/Classique/Text.hpp Game/UI/Classique/Sprite.cpp Game/UI/Classique/Sprite.hpp Game/UI/Button/ItemButton.hpp)

# TODO : Temporary solution, copy is not done each time the project is compiled
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(Game Engine)

install(TARGETS Game RUNTIME DESTINATION bin)
