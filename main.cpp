#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

#include "shader.hpp"

using namespace std;

// global
float glo_windowWidth = 600.f;
float glo_windowHeight = 600.f;
float glo_ratio = glo_windowWidth/glo_windowHeight;

//Called when the window is resized
void GLFWCALL handleResize(int width,int height)
{
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
    glo_windowWidth = static_cast<float>(width);
    glo_windowHeight = static_cast<float>(height);
    glo_ratio = glo_windowWidth/glo_windowHeight;
    //Set the camera perspective
    glLoadIdentity(); //reset the camera
    gluPerspective( 60.0f, glo_ratio, 1.0f, 100.0f );
}

int main()
{
   int running = GL_TRUE;

   // Initialize GLFW
   if( !glfwInit() )
   {
       exit( EXIT_FAILURE );
   } else {
       // Get GLFW Version
       int majorGLFW, minorGLFW, revGLFW;
       glfwGetVersion(&majorGLFW, &minorGLFW, &revGLFW);
       cout << "GLFW Version " << majorGLFW << "." << minorGLFW << "." << revGLFW << " loaded." << endl;
   }

   // Open an OpenGL window
   glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
   if( !glfwOpenWindow( glo_windowWidth, glo_windowHeight, 0,0,0, 0,0,0, GLFW_WINDOW ) )
   {
       glfwTerminate();
       exit( EXIT_FAILURE );
   } else {
       // Get OpenGL Version
       int majorGL = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
       int minorGL = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);
       cout << "OpenGL Version " << majorGL << "." << minorGL << " loaded." << endl;
   }

   // Initialize Glew
   GLenum glewInitCode = glewInit();
   if (GLEW_OK != glewInitCode)
   {
       cout << "Glew error : " << glewGetErrorString(glewInitCode) << endl;
       exit( EXIT_FAILURE );
   } else {
       cout << "GLEW Version " << glewGetString(GLEW_VERSION) << " loaded." << endl;
   }

   if ( !glewIsSupported("GL_ARB_vertex_buffer_object") )
   {
       cout << "Glew : GL_ARB_vertex_buffer_object not supported" << endl;
       exit( EXIT_FAILURE );
   }
   // Callbacks
   glfwSetWindowSizeCallback(handleResize);

   // Creating geometry

   // This will identify our vertex buffer
   GLuint vertexbuffer;

   // Generate 1 buffer, put the resulting identifier in vertexbuffer
   glGenBuffers(1, &vertexbuffer);

   // The following commands will talk about our 'vertexbuffer' buffer
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

   // An array of 3 vectors which represents 3 vertices
   static const GLfloat g_vertex_buffer_data[] =
   {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,
   };

   // Give our vertices to OpenGL.
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

   // Create and compile our GLSL program from the shaders
   GLuint programID = LoadShaders( "../shader/SimpleVertexShader.vertexshader", "../shader/SimpleFragmentShader.fragmentshader" );
   cout << "Shader loaded." << endl;

   // Get a handle for our buffers
   GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");


   // Get a handle for our "MVP" uniform
   GLuint MatrixID = glGetUniformLocation(programID, "MVP");

   // Projection matrix : 60° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   glm::mat4 Projection = glm::perspective(60.0f, glo_ratio, 1.f, 100.0f);
   // Camera matrix
   glm::mat4 View       = glm::lookAt(
                               glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                               glm::vec3(0,0,0), // and looks at the origin
                               glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                          );
   // Model matrix : an identity matrix (model will be at the origin)
   glm::mat4 Model      = glm::mat4(1.0f);
   // Our ModelViewProjection : multiplication of our 3 matrices
   glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

   cout << "Starting main loop." << endl;
   glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
   // Main loop
   do
   {
       // Check if ESC key was pressed or window was closed
       running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
       // OpenGL rendering goes here...
       glClear( GL_COLOR_BUFFER_BIT );

       // Rendering
       {
           // Use our shader
           glUseProgram(programID);

           // Send our transformation to the currently bound shader,
           // in the "MVP" uniform
           glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

           // 1rst attribute buffer : vertices
           glEnableVertexAttribArray(0);
           glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
           glVertexAttribPointer(
               vertexPosition_modelspaceID, // The attribute we want to configure
               3,                  // size
               GL_FLOAT,           // type
               GL_FALSE,           // normalized?
               0,                  // stride
               (void*)0            // array buffer offset
           );

           // Draw the triangle !
           glDrawArrays(GL_TRIANGLES, 0, 3); // From index 0 to 3 -> 1 triangle

           glDisableVertexAttribArray(0);
       }

       // Swap front and back rendering buffers
       glfwSwapBuffers();
   } while (running);

   // Close window and terminate GLFW
   glfwTerminate();

   // Cleanup VBO
   glDeleteBuffers(1, &vertexbuffer);

   // Exit program
   exit( EXIT_SUCCESS );
}

