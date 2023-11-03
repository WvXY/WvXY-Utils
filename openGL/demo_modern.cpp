#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>


/*===============================================================*/
// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* -------------------VAO, VBO---------------------*/
std::string vertexShaderSource =
        "#version 330\n"
        "in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

std::string fragmentShaderSource =
        "#version 330\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "  FragColor = vec4(0.0f, 1.f, 0.f, 1.0f);\n"
        "}\n";

float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
};


void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

/* -----------------functions-------------------*/

// glfw initialization
GLFWwindow *init_glfw() {
  if (!glfwInit()) {
    std::cout << stderr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl 3.X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // opengl x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4); // anti-aliasing
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // create window
  GLFWwindow* window = glfwCreateWindow(
            SCR_WIDTH,
            SCR_HEIGHT,
            "demo",
            NULL,
            NULL);

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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

//===============================================================//

unsigned int make_vao(float verts[]) {
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  return VAO;
}


unsigned int compile_shader(std::string source, int type) {
  unsigned int shader = glCreateShader(type); // type = GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
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


unsigned int create_program(unsigned int vertexShader, unsigned int fragmentShader) {
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

//  glDeleteShader(vertexShader);
//  glDeleteShader(fragmentShader);

  return shaderProgram;
}

/*===============================================================*/
/*  ----------------main loop---------------- */
void draw(GLFWwindow *window, unsigned int shaderProgram, unsigned int VAO)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  std::cout << VAO << std::endl;

  glDrawArrays(GL_TRIANGLES, 0, 3);
  std::cout << "Here4" << std::endl;
  glfwPollEvents();
  glfwSwapBuffers(window);
}


/* program entry */
int main()
{
  GLFWwindow* window = init_glfw();
  init_glad();

  unsigned int vertexShader, fragmentShader, shaderProgram;
  vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
  fragmentShader = compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  shaderProgram = create_program(vertexShader, fragmentShader);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0); // this creates a canvas you can do 2D drawing on



  unsigned int VAO = make_vao(vertices);

  int i = 0;
  // render loop
  while (!glfwWindowShouldClose(window)) {
//    process_input(::window);
    std::cout << ++i << std::endl;

    draw(window, shaderProgram, VAO);

//    glClearColor(1.f, 1.f, 1.f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // Use the shader program and bind the VAO
//    glUseProgram(shaderProgram);
//    VAO = make_vao(vertices);
//    glBindVertexArray(VAO);
//
//    // Draw the triangle
//    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers and poll for events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}