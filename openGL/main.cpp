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
        1, 3, 4, 0, 1, 4,
    };

    wvxy::GlUtils gl_utils{600, 800};

    while (!glfwWindowShouldClose(gl_utils.window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        gl_utils.Draw(vertices, indices, sizeof(vertices), sizeof(indices));

        glfwSwapBuffers(gl_utils.window);
        glfwPollEvents();
    }

    return 0;
}