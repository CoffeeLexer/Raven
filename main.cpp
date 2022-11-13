#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


#include <iostream>
#include <unordered_map>
#include <unordered_set>


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

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;

    bool operator==(const Vertex& other) const {
        return pos == other.pos && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return (hash<glm::vec3>()(vertex.pos) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    // INIT IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Load Model
    const char* MODEL_PATH = "assets/objects/box_01/object.obj";

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH)) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

//            if (uniqueVertices.count(vertex) == 0) {
//                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
//                vertices.push_back(vertex);
//            }

            vertices.push_back(vertex);
//            indices.push_back(uniqueVertices[vertex]);
            indices.push_back(vertices.size());
        }
    }

    printf("Vert: %zu\n", vertices.size());
    printf("Indi: %zu\n", indices.size());

    float rawVertices[] = {
            0.5f,  0.5f, -1.0f,
            0.5f, -0.5f, -1.0f,
            -0.5f, -0.5f, +1.0f,
            -0.5f,  0.5f, +1.0f
    };
    float textureCoordinates[] = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
    };
    unsigned int rawIndices[] = {
            0, 1, 3,
            1, 2, 3
    };
    unsigned int indices2[] = {
            1, 2, 3
    };
    unsigned int VBO_position; // Vertex buffer object => rawVertices locations
    unsigned int VBO_texture; // Vertex buffer object => rawVertices locations
    unsigned int VAO1; // Vertex array object => Relations between GPU buffers
    unsigned int EBO; // Element array buffer => Indexation order of vertex buffer

    // generate vertex array
    glGenVertexArrays(1, &VAO1);
    // generate any buffer
    glGenBuffers(1, &VBO_position);
    //glGenBuffers(1, &VBO_texture);
    // generate any buffer
    glGenBuffers(1, &EBO);

    // bind vertex array buffer
    glBindVertexArray(VAO1);

    // bind vertex buffer object and upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(vertices.size()), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)));

//    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);

//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // bind element array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(indices.size()), indices.data(), GL_STATIC_DRAW);

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
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection
    float fov = 45;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool isRotating = true;
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
            ImGui::SameLine();
            ImGui::Checkbox("Auto Rotate", &isRotating);
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

        if(isRotating) {
            rotate += 1;
            rotate = static_cast<float>(static_cast<int>(rotate) % 360);
        }

        glm::mat4 model(1.0f);
        model = glm::translate(model, translate);
        model = glm::rotate(model, glm::radians(rotate), up);
        model = glm::scale(model, scale);

        glm::mat4 view = glm::lookAt(cameraPosition, cameraTarget, up);

        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        s2.use();
        s2.set(mvp);
//        s2.set(glm::mat4(1.0f));

        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //##############################################################################################################
        glfwSwapBuffers(window);
    }

    // Shutdown system
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO_position);
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
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}