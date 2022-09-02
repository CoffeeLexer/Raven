cmake_minimum_required(VERSION 3.21)

project(Raven VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Raven main.cpp)

add_subdirectory(external/glad)
target_include_directories(${PROJECT_NAME} PRIVATE external/glad/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glad/src)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glm)
target_include_directories(${PROJECT_NAME} PRIVATE external/glm/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glm/src)
target_link_libraries(${PROJECT_NAME} glm)

add_subdirectory(external/spirv-headers)
target_include_directories(${PROJECT_NAME} PRIVATE external/spirv-headers/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/spirv-headers/src)
target_link_libraries(${PROJECT_NAME} SPIRV-Headers)



add_subdirectory(external/spirv-tools)
target_include_directories(${PROJECT_NAME} PRIVATE external/include/spirv-tools)
target_link_directories(${PROJECT_NAME} PRIVATE external/spirv-tools/source)
target_link_libraries(${PROJECT_NAME} spirv-tools)



add_subdirectory(external/glslang)
target_include_directories(${PROJECT_NAME} PRIVATE external/glslang/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glslang/src)
target_link_libraries(${PROJECT_NAME} glslang)

set(SHADERC_SKIP_TESTS TRUE)

include(external/shaderc/cmake/utils.cmake)

#add_subdirectory(external/shaderc)
#target_include_directories(${PROJECT_NAME} PRIVATE external/shaderc/include)
#target_link_directories(${PROJECT_NAME} PRIVATE external/shaderc/src)
#target_link_libraries(${PROJECT_NAME} shaderc)

add_subdirectory(external/shaderc/glslc)
target_include_directories(${PROJECT_NAME} PRIVATE external/glslc/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glslc/src)
target_link_libraries(${PROJECT_NAME} glslc)