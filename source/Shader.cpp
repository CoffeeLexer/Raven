#include <string>
#include <glad/glad.h>
#include <unordered_map>
#include <stdexcept>
#include <fstream>

#include "FileSystem.h"

#include "Shader.h"

static std::unordered_map<std::string, GLenum> const stringToShaderType = {
        {"vert", GL_VERTEX_SHADER},
        {"frag", GL_FRAGMENT_SHADER},
        {"tesc", GL_TESS_CONTROL_SHADER},
        {"tese", GL_TESS_EVALUATION_SHADER},
        {"geom", GL_GEOMETRY_SHADER},
        {"comp", GL_COMPUTE_SHADER}
};

Shader::Shader() {
    shaderProgram = glCreateProgram();
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

Shader Shader::add(const std::string &path) {
    GLenum shaderType;
    try {
        shaderType = stringToShaderType.at(Raven::FileSystem::getExtension(path));
    }
    catch (const std::exception &e) {
        std::string error = "ERROR::SHADER::ADD::EXTENSION\n\tIncompatible file extension: ";
        error.append(path);
        throw std::runtime_error(error);
    }

    std::ifstream file;
    file.open(path);

    if(file.bad()) {
        std::string error = "ERROR::SHADER:ADD::FILE\n\tCould not open the file: ";
        error.append(path);
        throw std::runtime_error(error);
    }

    std::string str;

    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    file.close();

    const char* source = str.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int params;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    if (!params)
    {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &params);
        char infoLog[params];
        glGetShaderInfoLog(shader, params, nullptr, infoLog);
        std::string error = "ERROR::SHADER::ADD::COMPILATION_FAILED\n";
        error.append(infoLog);
        throw std::runtime_error(error);
    }

    glAttachShader(shaderProgram, shader);
    glDeleteShader(shader);

    return *this;
}

void Shader::build() const {
    glLinkProgram(shaderProgram);

    int params;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
    if (!params) {
        glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &params);
        char infoLog[params];
        glGetProgramInfoLog(shaderProgram, params, nullptr, infoLog);
        std::string error = "ERROR::SHADER::BUILD::LINKING_FAILED\n";
        error.append(infoLog);
        throw std::runtime_error(error);
    }
}

unsigned int Shader::detach() {
    unsigned int result = shaderProgram;
    shaderProgram = 0;
    return result;
}

void Shader::use() const {
    glUseProgram(shaderProgram);
}