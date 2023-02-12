cmake_minimum_required(VERSION 3.21)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(PoposibEngine)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(Engine)
add_subdirectory(Game)