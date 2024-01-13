#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
//        glm::vec2 texCoord;
};

class GlUtils {
  public:
    GlUtils(std::string window_name, int screen_width, int screen_height);
    ~GlUtils();
    GlUtils(const GlUtils&) = delete;
    GlUtils& operator=(const GlUtils&) = delete;

    GLFWwindow* window{};

    void Draw(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);
    void Run();

  private:
    int SCR_WIDTH{800};
    int SCR_HEIGHT{600};
    const std::string windowName;

    unsigned int VBO{}, VAO{}, EBO{};
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    unsigned int shaderProgram{};

    void Init();
    GLFWwindow* InitGLFW();
    void InitGLAD();
    void InitViewport();

    std::string ReadShaderSource(const std::string& path);
    unsigned int CompileShader(std::string& source, int type);
    unsigned int CreateProgram(unsigned int vertexShader,
                               unsigned int fragmentShader);

    bool hasTexture{false};
    bool hasIndices{false};
    void CreateBuffer(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);
    void BindBuffer();

};
} // namespace wvxy
