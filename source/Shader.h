#ifndef RAVEN_SHADER_H
#define RAVEN_SHADER_H

#include <vector>

class Shader {
    unsigned int shaderProgram = 0;
public:
    Shader();
    ~Shader();
    Shader add(const std::string &path);
    void use() const;
    void build() const;
    unsigned int detach();
};

#endif //RAVEN_SHADER_H