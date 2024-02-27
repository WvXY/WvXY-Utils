#include "gl_utils.hpp"

int main() {
    float vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, // top left
        -0.3f, 0.7f,  0.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3, // second Triangle
        1, 3, 4, // random
        0, 1, 4
    };

    wvxy::GlUtils glUtils{"Hello GL", 800, 600};
    GLFWwindow* window = glUtils.window;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUtils.Draw(vertices, indices, sizeof(vertices), sizeof(indices));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}