set(EDITOR_HEADERS
        Editor/Editor.hpp
        )

set(EDITOR_SOURCES
        Editor/Editor.cpp
        )

add_library(Editor STATIC ${EDITOR_HEADERS} ${EDITOR_SOURCES})