cmake_minimum_required(VERSION 3.23)
project(Raven VERSION 0.0.1)

#   compiling settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

#   find and add all source files
set(SOURCE_PATH source)
file(GLOB_RECURSE PROJECT_SOURCES ${SOURCE_PATH}/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${SOURCE_PATH}/*.h)

add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES} ${PROJECT_HEADERS})

#   github.com/Dav1dde/glad
add_subdirectory(external/glad)
target_include_directories(${PROJECT_NAME} PRIVATE external/glad/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glad/src)
target_link_libraries(${PROJECT_NAME} glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#   github.com/glfw/glfw
add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE external/glfw/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)
target_link_libraries(${PROJECT_NAME} glfw)

# github.com/ocornut/imgui.git
file(GLOB IMGUI_SOURCES "external/imgui/*.cpp")
file(GLOB IMGUI_HEADERS "external/imgui/*.h")
add_library(IMGUI ${IMGUI_SOURCES} ${IMGUI_HEADERS} external/imgui/backends/imgui_impl_glfw.cpp external/imgui/backends/imgui_impl_opengl3.cpp)

target_include_directories(IMGUI PUBLIC external/imgui)
target_include_directories(IMGUI PUBLIC . .. external/imgui/backends)

target_link_libraries(${PROJECT_NAME} IMGUI)


#   github.com/g-truc/glm
add_subdirectory(external/glm)
target_include_directories(${PROJECT_NAME} PRIVATE external/glm/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/glm/src)
target_link_libraries(${PROJECT_NAME} glm)

#   github.com/tinyobjloader/tinyobjloader
add_subdirectory(external/tinyobjloader)
target_include_directories(${PROJECT_NAME} PRIVATE external/tinyobjloader/include)
target_link_directories(${PROJECT_NAME} PRIVATE external/tinyobjloader/src)
target_link_libraries(${PROJECT_NAME} tinyobjloader)

#   github.com/nothings/stb
add_library(STB INTERFACE)
file(GLOB STB_FILES "external/stb/*.h")
target_sources(STB INTERFACE
        FILE_SET HEADERS
        BASE_DIRS external/stb/
        FILES ${STB_FILES})
target_link_libraries(${PROJECT_NAME} STB)

#   find glslc shader compiled (github.com/google/shaderc, also included in VulkanSDK)
#       compiling glslc inside project uses tons of memory
find_program(glslc glslc)

#   define supported shader stages by corresponding file extensions
list(APPEND SHADER_EXTENSIONS "vert")
list(APPEND SHADER_EXTENSIONS "frag")
list(APPEND SHADER_EXTENSIONS "tesc")
list(APPEND SHADER_EXTENSIONS "tese")
list(APPEND SHADER_EXTENSIONS "geom")
list(APPEND SHADER_EXTENSIONS "comp")

#   define shader input/output paths
set(SHADER_PATH_SOURCES assets/shaders/sources)
set(SHADER_PATH_BINARIES assets/shaders/binaries)

#   convert shader paths to absolute path for nested folder purposes
cmake_path(ABSOLUTE_PATH SHADER_PATH_SOURCES)
cmake_path(ABSOLUTE_PATH SHADER_PATH_BINARIES)

#   Find all shader source files and construct file path list
foreach(EXTENSION IN LISTS SHADER_EXTENSIONS)
    file(GLOB_RECURSE FILES ${SHADER_PATH_SOURCES}/*.${EXTENSION})
    list(APPEND SHADER_SOURCE_FILES ${FILES})
endforeach()

#   Convert input file paths into file output paths for compiling purposes
foreach(SOURCE IN LISTS SHADER_SOURCE_FILES)
    string(REPLACE "${SHADER_PATH_SOURCES}" "${SHADER_PATH_BINARIES}" BINARY ${SOURCE})
    add_custom_command(
            OUTPUT ${BINARY}.spv
            DEPENDS ${_SOURCE}
            COMMAND ${glslc}
            ${SOURCE}
            -o ${BINARY}.spv
            --target-env=opengl
            -x glsl
            COMMENT "Compiling ${SOURCE}"
    )
    list(APPEND SHADER_BINARIES ${BINARY}.spv)
endforeach()

add_custom_target(TARGET_SHADERS ALL DEPENDS ${SHADER_BINARIES})
add_dependencies(${PROJECT_NAME} TARGET_SHADERS)

# Copy assets to build directory
file(GLOB_RECURSE ASSETS RELATIVE ${CMAKE_SOURCE_DIR} "assets/*.*")
foreach(ASSET ${ASSETS})
    message(STATUS ${ASSET})
    file(GENERATE OUTPUT ${ASSET} INPUT ${ASSET})
endforeach()


##   prerequisites for CPack
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#include(InstallRequiredSystemLibraries)
#
##   license definition for CPack
#set(LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/license.txt)
#if(EXISTS ${LICENSE})
#    message(STATUS "Using license: ${LICENSE}")
#    set(CPACK_RESOURCE_FILE_LICENSE ${LICENSE})
#else()
#    message(FATAL "LICENSE IS NOT FOUND IN PATH: ${LICENSE}")
#endif()

##   rest of CPack setting
#set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
#
##   cpack creation initialization
#include(CPACK)