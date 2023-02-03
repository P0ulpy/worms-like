set(GAME_HEADERS
        src/main.hpp
)

set(GAME_SOURCES
        src/main.cpp
)

# TODO : Temporary solution, copy is not done each time the project is compiled
# file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Game ${GAME_HEADERS} ${GAME_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/Engine)

target_link_libraries(Game Engine)

install(TARGETS Game RUNTIME DESTINATION bin)
