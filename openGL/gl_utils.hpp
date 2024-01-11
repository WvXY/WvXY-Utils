#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

namespace wvxy {

class GlUtils {
  public:
    GlUtils(int screen_width, int screen_height);
    ~GlUtils();
    GlUtils(const GlUtils&) = delete;
    GlUtils& operator=(const GlUtils&) = delete;

    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;
    GLFWwindow* window;

    void Draw(float* vertices, unsigned int* indices, size_t size_verts,
              size_t size_indices);

    void Run();

  private:
    unsigned int VBO, VAO, EBO;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    unsigned int shaderProgram;

    void Init();
    GLFWwindow* InitGLFW();
    void InitGLAD();
    void InitViewport();

    std::string ReadShaderSource(const std::string path);
    unsigned int CompileShader(std::string& source, int type);
    unsigned int CreateProgram(unsigned int vertexShader,
                               unsigned int fragmentShader);

    void CreateBuffer(float& verts, unsigned int& indices, size_t size_verts,
                      size_t size_indices);
    void BindBuffer();


};
} // namespace wvxy
