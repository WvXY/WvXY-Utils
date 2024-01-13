#include "gl_utils.hpp"

#include <vector>


int main() {
    std::vector<uint16_t> indices = {0, 1, 3, // first Triangle
                                     1, 2, 3, // second Triangle
                                     1, 3, 4, // random
                                     0, 1, 4};

    std::vector<wvxy::Vertex> vertices = {
        {{0.5f, 0.5f, 0.0f},   {1.f, 0.f, 0.f}},
        {{0.5f, -0.5f, 0.0f},  {0.f, 1.f, 0.f}},
        {{-0.5f, -0.5f, 0.0f}, {0.f, 0.f, 1.f}},
        {{-0.5f, 0.5f, 0.0f},  {1.f, 1.f, 0.f}},
        {{-0.3f, 0.7f, 0.0f},  {0.f, 1.f, 1.f}}
    };

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
