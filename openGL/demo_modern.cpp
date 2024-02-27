#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* -------------------VAO, VBO---------------------*/
std::string vertexShaderSource = "#version 460\n"
                                 "in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos, 1.0);\n"
                                 "}\n";

std::string fragmentShaderSource = "#version 460\n"
                                   "layout(location = 0) out vec4 fragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "  fragColor = vec4(0.0f, 1.f, 0.f, 1.0f);\n"
                                   "}\n";

float vertices[] = {
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f  // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// glfw initialization
GLFWwindow* init_glfw() {
    if (!glfwInit()) {
        std::cout << stderr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES,
                   4); // anti-aliasing
                       //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "demo", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void init_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
}

//===============================================================//
unsigned int compile_shader(std::string& source, int type) {
    unsigned int shader =
        glCreateShader(type); // type = GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
    auto src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    // error handling
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << stderr << "Failed to compile shader" << std::endl;
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << stderr << infoLog << std::endl;
    }

    return shader;
}

unsigned int create_program(unsigned int& vertexShader,
                            unsigned int& fragmentShader) {
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // error handling
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        std::cout << stderr << "Failed to link shader program" << std::endl;
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << stderr << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void setup_buffers(float& verts, unsigned int& indices, size_t size_verts,
                   size_t size_indices, unsigned int& VAO) {
    unsigned int VBO, EBO;
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

    //  return VAO;
}
/*===============================================================*/
/*  ----------------main loop---------------- */
void draw(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
}

/* program entry */
int main() {
    GLFWwindow* window = init_glfw();
    init_glad();

    unsigned int vertexShader, fragmentShader, shaderProgram;
    vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader = compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    shaderProgram = create_program(vertexShader, fragmentShader);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
    unsigned int VAO, VBO, EBO;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        size_t size_verts = sizeof(vertices);
        size_t size_indices = sizeof(indices);
        setup_buffers(*vertices, *indices, size_verts, size_indices, VAO);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
