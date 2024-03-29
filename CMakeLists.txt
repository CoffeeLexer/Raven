cmake_minimum_required(VERSION 3.24)

# SetUp Directory
file (GLOB_RECURSE DS_STORE_FILES ${CMAKE_SOURCE_DIR}/*.DS_Store)
foreach (E ${DS_STORE_FILES})
    cmake_path (RELATIVE_PATH E BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE R)
    message (STATUS "REMOVING ${R}")
    file (REMOVE ${E})
endforeach ()

project(Raven C CXX)

set(CMAKE_CXX_STANDARD 20)

set(FILE_PATH ${CMAKE_SOURCE_DIR})
set(SHADER_PATH ${CMAKE_SOURCE_DIR}/asset/shader/)
set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/asset/texture/)
set(MODEL_PATH ${CMAKE_SOURCE_DIR}/asset/model/)
configure_file(config.h.in config.h)

find_package(Vulkan REQUIRED)
find_library(QuartzCore_Library QuartzCore)
find_library(Metal_Library Metal)

add_subdirectory(external/glm EXCLUDE_FROM_ALL)
add_subdirectory(external/glfw EXCLUDE_FROM_ALL)
add_subdirectory(external/tinyobjloader EXCLUDE_FROM_ALL)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad_opengl REPRODUCIBLE LOADER API gl:core=4.0)
glad_add_library(glad_vulkan REPRODUCIBLE LOADER API vulkan=1.0)

file(GLOB_RECURSE SOURCE ${CMAKE_SOURCE_DIR}/source/*.cpp ${CMAKE_SOURCE_DIR}/source/*.hpp ${CMAKE_SOURCE_DIR}/source/*.mm)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE})

include_directories(external/stb)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyobjloader)
target_link_libraries(${PROJECT_NAME} PRIVATE glad_opengl)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# Doesn't work on macOS
target_link_libraries(${PROJECT_NAME} PRIVATE glad_vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE ${QuartzCore_Library})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Metal_Library})

file(GLOB_RECURSE SHADERS ${CMAKE_SOURCE_DIR}/shader/*)

