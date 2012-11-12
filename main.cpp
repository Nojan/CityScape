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

   // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
   // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
   static const GLfloat g_vertex_buffer_data[] = {
       -1.0f,-1.0f,-1.0f,
       -1.0f,-1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
       -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
       -1.0f,-1.0f, 1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
       -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f,-1.0f,
       -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
   };

   // One color for each vertex. They were generated randomly.
   static const GLfloat g_color_buffer_data[] = {
       0.583f,  0.771f,  0.014f,
       0.609f,  0.115f,  0.436f,
       0.327f,  0.483f,  0.844f,
       0.822f,  0.569f,  0.201f,
       0.435f,  0.602f,  0.223f,
       0.310f,  0.747f,  0.185f,
       0.597f,  0.770f,  0.761f,
       0.559f,  0.436f,  0.730f,
       0.359f,  0.583f,  0.152f,
       0.483f,  0.596f,  0.789f,
       0.559f,  0.861f,  0.639f,
       0.195f,  0.548f,  0.859f,
       0.014f,  0.184f,  0.576f,
       0.771f,  0.328f,  0.970f,
       0.406f,  0.615f,  0.116f,
       0.676f,  0.977f,  0.133f,
       0.971f,  0.572f,  0.833f,
       0.140f,  0.616f,  0.489f,
       0.997f,  0.513f,  0.064f,
       0.945f,  0.719f,  0.592f,
       0.543f,  0.021f,  0.978f,
       0.279f,  0.317f,  0.505f,
       0.167f,  0.620f,  0.077f,
       0.347f,  0.857f,  0.137f,
       0.055f,  0.953f,  0.042f,
       0.714f,  0.505f,  0.345f,
       0.783f,  0.290f,  0.734f,
       0.722f,  0.645f,  0.174f,
       0.302f,  0.455f,  0.848f,
       0.225f,  0.587f,  0.040f,
       0.517f,  0.713f,  0.338f,
       0.053f,  0.959f,  0.120f,
       0.393f,  0.621f,  0.362f,
       0.673f,  0.211f,  0.457f,
       0.820f,  0.883f,  0.371f,
       0.982f,  0.099f,  0.879f
   };

   GLuint vertexbuffer;
   glGenBuffers(1, &vertexbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

   GLuint colorbuffer;
   glGenBuffers(1, &colorbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

   // Create and compile our GLSL program from the shaders
   GLuint programID = LoadShaders( "../shader/SimpleVertexShader.vertexshader", "../shader/SimpleFragmentShader.fragmentshader" );
   cout << "Shader loaded." << endl;

   // Get a handle for our buffers
   GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
   GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");

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

   // OpenGL Setting
   glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);

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
               3,                           // size
               GL_FLOAT,                    // type
               GL_FALSE,                    // normalized?
               0,                           // stride
               (void*)0                     // array buffer offset
           );

           // 2nd attribute buffer : colors
           glEnableVertexAttribArray(1);
           glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
           glVertexAttribPointer(
               vertexColorID,               // The attribute we want to configure
               3,                           // size
               GL_FLOAT,                    // type
               GL_FALSE,                    // normalized?
               0,                           // stride
               (void*)0                     // array buffer offset
           );

           // Draw the triangleS !
           glDrawArrays(GL_TRIANGLES, 0, 12*3); // From index 0 to 12*3 -> 12 triangles

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

