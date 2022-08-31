cmake_minimum_required(VERSION 3.21)

project(Raven VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Raven main.cpp)

add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)
