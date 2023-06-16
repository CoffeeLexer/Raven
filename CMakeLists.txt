cmake_minimum_required(VERSION 3.20)
project(Raven C CXX)

add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad REPRODUCIBLE LOADER API gl:core=4.0)

add_subdirectory(external/assimp EXCLUDE_FROM_ALL)
add_subdirectory(external/tinyobjloader EXCLUDE_FROM_ALL)

set(SOURCE_PATH source)
file(GLOB_RECURSE PROJECT_SOURCES ${SOURCE_PATH}/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${SOURCE_PATH}/*.h)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader)