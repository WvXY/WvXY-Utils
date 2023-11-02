// #include <stdlib.h>
// #include <stdio.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>


/*===============================================================*/
/* ----initialization---- */
GLFWwindow *window;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void init_glfw();
void init_glad();

/*===============================================================*/
/*  ----------------main loop---------------- */
void render_loop()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(10);
    glLineWidth(2.5);

    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
    };
}

/* program entry */
int main(int argc, char *argv[])
{
    init_glfw();
    init_glad();

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // set up view
    glViewport(0, 0, 400, 400);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // see https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
    glOrtho(0.0, 400.0, 0.0, 400.0, 0.0, 1.0); // this creates a canvas you can do 2D drawing on

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Draw
        render_loop();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}


/* -----------------functions-------------------*/

// glfw initialization
void init_glfw()
{
  if (!glfwInit())
  {
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
  window = glfwCreateWindow(
          SCR_WIDTH,
          SCR_HEIGHT,
          "demo",
          NULL,
          NULL);

  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}


void init_glad()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}



void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *_window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


/*===============================================================*/
/* -------------------VAO, VBO---------------------*/
const char *vertexShaderSource =
    "#version 410\n"
    "in vec3 vp;\n"
    "void main()\n"
    "{"
    "    gl_Position = vec4(vp, 1.0);"
    "}";

const char *fragmentShaderSource =
    "#version 410\n"
    "//in vec3 vp;\n"
    "out vec4 frag_colour;\n"
    "void main()\n"
    "{"
    "frag_colour = vec4(1, 1, 1, 1);"
    "}";

// working on this
int make_vao(int verts)
{
    
}


unsigned int compile_shader()
{
    
}