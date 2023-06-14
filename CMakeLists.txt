cmake_minimum_required(VERSION 3.20)
project(Raven C CXX)

add_subdirectory(external/glfw EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)