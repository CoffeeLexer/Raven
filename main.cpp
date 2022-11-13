#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include "external/imgui/imgui.h"
#include "external/imgui/backends/imgui_impl_glfw.h"
#include "external/imgui/backends/imgui_impl_opengl3.h"

#include "source/Shader.h"
#include "source/Image.h"
#include "source/Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
    // Init glfw
    glfwInit();
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MAC OS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW Window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project Raven", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE); // PRESS-HOLD = 1 press, no refresh

    // Bind glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float vertices[] = {
            0.5f,  0.5f, -1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, +1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, +1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    unsigned int indices2[] = {
            1, 2, 3
    };
    unsigned int VBO; // Vertex buffer object => vertices locations
    unsigned int VAO1; // Vertex array object => Relations between GPU buffers
    unsigned int EBO; // Element array buffer => Indexation order of vertex buffer

    // generate vertex array
    glGenVertexArrays(1, &VAO1);
    // generate any buffer
    glGenBuffers(1, &VBO);
    // generate any buffer
    glGenBuffers(1, &EBO);

    // bind vertex array buffer
    glBindVertexArray(VAO1);

    // bind vertex buffer object and upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    unsigned int EBO2;
    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    Raven::Shader s1 = Raven::Shader();
    s1.add("assets/shaders/sources/simple/shader.vert");
    s1.add("assets/shaders/sources/simple/shader.frag");
    s1.build();

    Raven::Shader s2("assets/shaders/sources/simple/shader.vert", "assets/shaders/sources/simple/shader.frag");

    Raven::Image img("assets/images/wood.jpg");

//
//    const float radius = 10.0f;
//    float camX = sin(glfwGetTime()) * radius;
//    float camZ = cos(glfwGetTime()) * radius;
//    glm::mat4 view;
//    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), cameraTarget, up);

    Raven::Texture tex("assets/images/wood.jpg");

    glActiveTexture(GL_TEXTURE0);
    tex.use();

    // Model
    glm::vec3 translate(0.0f, 0.0f, 0.0f);
    float rotate = 0.0f;
    glm::vec3 scale(1.0f, 1.0f, 1.0f);

    // View
    glm::vec3 cameraPosition = glm::vec3(4.0f, 3.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection
    float fov = 45;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool a = true;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {

            ImGui::Begin("Master Debugger!");

            ImGui::Text("Model Matrix");
            ImGui::InputFloat3("Scale", glm::value_ptr(scale), "%.2f", 0);
            ImGui::InputFloat("Rotate", &rotate, 0.0f, 0.0f, "%.2f", 0);
            ImGui::InputFloat3("Translate", glm::value_ptr(translate), "%.2f", 0);

            ImGui::Text("View Matrix");
            ImGui::InputFloat3("Camera Position", glm::value_ptr(cameraPosition), "%.2f", 0);
            ImGui::InputFloat3("Camera Target", glm::value_ptr(cameraTarget), "%.2f", 0);

            ImGui::Text("Projection Matrix");
            ImGui::DragFloat("FOV", &fov, 1.0f, 0.0f, 180.0f, "%.3f", 0);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        //##############################################################################################################
        glClearColor(0.1f, 0.1f, 0.1f, 0.25f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //##############################################################################################################

        glm::mat4 model(1.0f);
        glm::translate(model, translate);
        glm::rotate(model, rotate, up);
        glm::scale(model, scale);
//        model = glm::mat4(1.0f);

        glm::mat4 view = glm::lookAt(cameraPosition, cameraTarget, up);

        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        s2.use();
        s2.set(mvp);
//        s2.set(glm::mat4(1.0f));

        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //##############################################################################################################
        glfwSwapBuffers(window);
    }

    // Shutdown system
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}