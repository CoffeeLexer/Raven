#pragma once

#include <filesystem>
#include <fstream>
#include <string>


namespace shader
{
    std::vector<char> load(const std::string shader);
};