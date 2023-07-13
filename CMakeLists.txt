cmake_minimum_required(VERSION 3.24)
project(Raven C CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad_opengl REPRODUCIBLE LOADER API gl:core=4.0)
glad_add_library(glad_vulkan REPRODUCIBLE LOADER API vulkan=1.0)

add_executable(${PROJECT_NAME} main.cpp source/example/example.hpp source/example/opengl.cpp source/engine.hpp source/engine/engine.cpp source/engine/queue_family_details.cpp source/engine/swapchain_details.cpp source/engine/shader.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad_opengl)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# Doesn't work on macOS
target_link_libraries(${PROJECT_NAME} PRIVATE glad_vulkan)

configure_file(${CMAKE_SOURCE_DIR}/shader/binary/frag.spv ${CMAKE_BINARY_DIR}/shader/binary/frag.spv COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shader/binary/vert.spv ${CMAKE_BINARY_DIR}/shader/binary/vert.spv COPYONLY)