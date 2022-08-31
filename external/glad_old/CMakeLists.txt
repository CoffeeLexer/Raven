file(GLOB sources "src/*.c" "src/*.cpp")
file(GLOB_RECURSE headers "include/*.h" "include/*.hpp")
add_library(glad ${sources} ${headers})
target_include_directories(glad PUBLIC include)