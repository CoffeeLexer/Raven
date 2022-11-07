#ifndef RAVEN_SHADER_H
#define RAVEN_SHADER_H

#include <vector>

class Shader {
    unsigned int shaderProgram = 0;
public:
    Shader();
    ~Shader();
    void add(const std::string &path);
    void use() const;
    void build() const;
    unsigned int detach();

    template<typename ...Shaders>
    Shader(const Shaders&... shaders) {
        shaderProgram = glCreateProgram();
        construct(shaders...);
    }
private:
    template<typename ...Shaders>
    void construct(const char* shader, Shaders... shaders) {
        add(shader);
        construct(shaders...);
    }
    void construct(const char* shader) {
        add(shader);
        build();
    }
};

#endif //RAVEN_SHADER_H