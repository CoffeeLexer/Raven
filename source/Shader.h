#ifndef RAVEN_SHADER_H
#define RAVEN_SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace Raven {
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

        void set(glm::mat4 mvp) {
            unsigned int location = glGetUniformLocation(shaderProgram, "mvp");
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mvp));
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
}

#endif //RAVEN_SHADER_H
