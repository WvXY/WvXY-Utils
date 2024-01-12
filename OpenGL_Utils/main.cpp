#include "gl_utils.hpp"

#include <vector>

int main() {
    std::vector<float> vertices = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, // top left
        -0.3f, 0.7f,  0.0f  // top left
    };

    std::vector<uint16_t> indices = {
        0, 1, 3, // first Triangle
        1, 2, 3, // second Triangle
        1, 3, 4, // random
        0, 1, 4
    };

    float vertices2[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, // top left
        -0.3f, 0.7f,  0.0f  // top left
    };
    std::cout << sizeof(vertices2)  << std::endl;
        std::cout << vertices.size() * sizeof(vertices[0]) << std::endl;

    wvxy::GlUtils glUtils{"Hello GL", 800, 600};
    GLFWwindow* window = glUtils.window;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUtils.Draw(vertices, indices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}