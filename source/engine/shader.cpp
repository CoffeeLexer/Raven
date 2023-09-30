#include "./shader.hpp"

#include <fstream>
#include <iostream>
#include "config.h"

int compile_vulkan(std::string input, std::string output)
{
    std::string cmd = "glslc " + input + " -o " + output + " --target-env vulkan1.0";
    return system(cmd.c_str());
}

std::vector<char> shader::load(const std::string shader)
{
    std::string request = shader;
    if (!std::filesystem::exists(request))
    {
        std::cerr << "FILE " + request + " NOT EXISTS!" << std::endl;
        throw std::runtime_error("UwU");
    }
    std::string compiled = shader + ".spv";
    compile_vulkan(request, compiled);

    std::ifstream file = std::ifstream(compiled, std::ios::binary | std::ios::in | std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0);
    std::vector<char> source(size);
    file.read(source.data(), size);
    file.close();

    std::filesystem::remove(compiled);
    return source;
}