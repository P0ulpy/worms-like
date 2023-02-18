set(GAME_HEADERS
        src/main.hpp
        Game/Game.hpp
        Game/Player/Player.hpp
        Game/Player/Creature.hpp
        Game/GameState/TurnState.hpp
        Game/GameState/GameState.hpp
)

set(GAME_SOURCES
        src/main.cpp
        Game/Game.cpp
        Game/Player/Player.cpp
        Game/Player/Creature.cpp
        Game/GameState/TurnState.cpp
        Game/GameState/GameState.cpp)

# TODO : Temporary solution, copy is not done each time the project is compiled
# file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/Game/Debug)
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(Game Engine)

install(TARGETS Game RUNTIME DESTINATION bin)
