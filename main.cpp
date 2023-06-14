#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* kVertSource = R"GLSL(
#version 400 core
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 bridgeColor;

void main()
{
    gl_Position = vec4(inPosition, 0.0f, 1.0f);
    bridgeColor = inColor;
}
)GLSL";

const char *kFragSource = R"GLSL(
#version 400 core
in vec3 bridgeColor;

out vec4 outColor;

void main()
{
    outColor = vec4(bridgeColor, 1.0f);
}
)GLSL";

const float kVertices[] = {
        -1.0f, +1.0f, 1.0f, 0.0f, 0.0f,
        +1.0f, +1.0f, 0.0f, 1.0f, 0.0f,
        +0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
};

const unsigned int kIndices[] = {
        0, 1, 2
};

int main()
{
    std::cout << "Start of execution\n";

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(250, 250, "Window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &kVertSource, nullptr);
    glCompileShader(vertShader);

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &kFragSource, nullptr);
    glCompileShader(fragShader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glUseProgram(program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(25.0f / 256.0f, 36.0f / 256.0f, 52.0f / 256.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    std::cout << "End   of execution\n";

    return 0;
}