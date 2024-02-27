#include <iostream>
#include <Eigen/Dense>

#include <GLFW/glfw3.h>
#include <gl/GL.h>

#include "stellar.h"
using namespace Stellar;


StellarMovement Earth(5.972e24, 6371e3, 60);  // dt = 60 seconds
StellarMovement Sun(1.989e30, 696340e3, 1);
Eigen::MatrixXf trajectory(3, 100000);

float windowWidth = 480;
float windowHeight = 480;

void render_loop(int &current_frame)
{
  glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPointSize(10);
  glLineWidth(2.5);

//  Eigen::Vector3f lastPosition = Earth.position;
  for(int i=0; i<100; i++) {
    Earth.getAcceleration(Sun);
    Earth.integrate();
  }
//  std::cout << "current frame: " << current_frame << std::endl;
  trajectory.col(current_frame) = Earth.position / AU * 400.f;

//  if (current_frame % 100 == 0){
//    std::cout << "Prev Pos: " << trajectory.col(current_frame-1) << std::endl;
//    std::cout << "Earth position: " << std::endl<< trajectory.col(current_frame) << std::endl;
//    std::cout << "Earth velocity: " << std::endl<< Earth.velocity << std::endl;
//  }
  std::cout << "Day: " << Earth.getCurrentDay() << std::endl;

  glColor3f(0.f, 0.f, 0.f);
  glBegin(GL_LINE_STRIP);
  for(int i=0; i<current_frame; i++) {
    glVertex3fv(trajectory.col(i).data());
  }
  glEnd();

  glBegin(GL_POINTS);
  glColor3f(1.f, 0.f, 0.f); // red sun
  glVertex3f(0.f, 0.f, 0.f);
  glColor3f(0.f, 0.f, 1.f); // blue earth
  glVertex3fv(trajectory.col(current_frame).data());
  glEnd();

  glFlush();
}


/* program entry */
int main(int argc, char *argv[])
{
  GLFWwindow* window;

  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  window = glfwCreateWindow(480, 480, "demo", NULL, NULL );
  if (!window)
  {
    fprintf( stderr, "Failed to open GLFW window\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval( 0 );

  // set up view
  glViewport( 0, 0, 480, 480);
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  // see https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
  glOrtho(-800., 800., -800., 800., 0.f, 1.f); // this creates a canvas you can do 2D drawing on


  Earth.position = Eigen::Vector3f(AU, 0, 0); // 1 AU right of the Sun
  Earth.velocity = Eigen::Vector3f(0, 29780.f, 0); // 29.78 km/s in y direction

  // init settings
  int current_frame = 0;
  // Main loop
  while( !glfwWindowShouldClose(window) )
  {
    // Draw

    render_loop(++current_frame);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Terminate GLFW
  glfwTerminate();

  // Exit program
  exit( EXIT_SUCCESS );
}

