#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "gl_utils.hpp"

namespace wvxy {

GlUtils::GlUtils(const std::string name, int screen_width, int screen_height)
    : SCR_WIDTH{screen_width}, SCR_HEIGHT{screen_height}, windowName{name}{
        Init();
}

GlUtils::~GlUtils() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void GlUtils::Init(){
    InitGLFW();
    InitGLAD();
    InitViewport();
    std::filesystem::path p = std::filesystem::current_path().parent_path();
    fragmentShaderSource =
        ReadShaderSource(p.string() + "/shaders/simple_shader.frag");
    vertexShaderSource =
        ReadShaderSource(p.string() + "/shaders/simple_shader.vert");
    auto vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    auto fragmentShader =
        CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    auto shaderProgram = CreateProgram(vertexShader, fragmentShader);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* GlUtils::InitGLFW() {
    if (!glfwInit()) {
        std::cout << stderr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // opengl 3.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // opengl x.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH,
                              SCR_HEIGHT,
                              windowName.c_str(),
                              nullptr,
                              nullptr);
    if (window == nullptr) {
        std::cout << stderr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    return window;
}

void GlUtils::InitGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << stderr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void GlUtils::InitViewport() { glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); }

std::string GlUtils::ReadShaderSource(const std::string path) {
    std::ifstream file{path, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        std::cout << "Error: File do not exist or could not open" << std::endl;
        throw std::runtime_error("Could not open file: " + path);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer.data();
}

unsigned int GlUtils::CompileShader(std::string& source, int type) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << stderr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    return shader;
}

unsigned int GlUtils::CreateProgram(unsigned int vertexShader,
                                    unsigned int fragmentShader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << stderr << "ERROR::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void GlUtils::CreateBuffer(float& verts, unsigned int& indices,
                           size_t size_verts, size_t size_indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_verts, &verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GlUtils::BindBuffer() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
}

void GlUtils::Draw(float* vertices, unsigned int* indices, size_t size_verts,
                   size_t size_indices) {
    CreateBuffer(*vertices, *indices, size_verts, size_indices);
    BindBuffer();
    glDrawElements(GL_TRIANGLES, size_indices / 4, GL_UNSIGNED_INT, nullptr);
}

void GlUtils::Run() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

} // namespace wvxy
