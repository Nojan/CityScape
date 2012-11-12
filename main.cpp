#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

#include "shader.hpp"
#include "texture.hpp"

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
   glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
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

   // Two UV coordinatesfor each vertex. They were created withe Blender.
   static const GLfloat g_uv_buffer_data[] = {
       0.000059f, 1.0f-0.000004f,
       0.000103f, 1.0f-0.336048f,
       0.335973f, 1.0f-0.335903f,
       1.000023f, 1.0f-0.000013f,
       0.667979f, 1.0f-0.335851f,
       0.999958f, 1.0f-0.336064f,
       0.667979f, 1.0f-0.335851f,
       0.336024f, 1.0f-0.671877f,
       0.667969f, 1.0f-0.671889f,
       1.000023f, 1.0f-0.000013f,
       0.668104f, 1.0f-0.000013f,
       0.667979f, 1.0f-0.335851f,
       0.000059f, 1.0f-0.000004f,
       0.335973f, 1.0f-0.335903f,
       0.336098f, 1.0f-0.000071f,
       0.667979f, 1.0f-0.335851f,
       0.335973f, 1.0f-0.335903f,
       0.336024f, 1.0f-0.671877f,
       1.000004f, 1.0f-0.671847f,
       0.999958f, 1.0f-0.336064f,
       0.667979f, 1.0f-0.335851f,
       0.668104f, 1.0f-0.000013f,
       0.335973f, 1.0f-0.335903f,
       0.667979f, 1.0f-0.335851f,
       0.335973f, 1.0f-0.335903f,
       0.668104f, 1.0f-0.000013f,
       0.336098f, 1.0f-0.000071f,
       0.000103f, 1.0f-0.336048f,
       0.000004f, 1.0f-0.671870f,
       0.336024f, 1.0f-0.671877f,
       0.000103f, 1.0f-0.336048f,
       0.336024f, 1.0f-0.671877f,
       0.335973f, 1.0f-0.335903f,
       0.667969f, 1.0f-0.671889f,
       1.000004f, 1.0f-0.671847f,
       0.667979f, 1.0f-0.335851f
   };

   GLuint vertexbuffer;
   glGenBuffers(1, &vertexbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

   GLuint uvbuffer;
   glGenBuffers(1, &uvbuffer);
   glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

   // Create and compile our GLSL program from the shaders
   GLuint programID = LoadShaders( "../shader/SimpleVertexShader.vertexshader", "../shader/SimpleFragmentShader.fragmentshader" );
   cout << "Shader loaded." << endl;

   // Get a handle for our buffers
   GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
   GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

   GLuint Texture = loadBMP_custom("../asset/uvtemplate.bmp");

   // Get a handle for our "myTextureSampler" uniform
   GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

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

           // Bind our texture in Texture Unit 0
           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, Texture);
           // Set our "myTextureSampler" sampler to user Texture Unit 0
           glUniform1i(TextureID, 0);

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

           // 2nd attribute buffer : UVs
           glEnableVertexAttribArray(1);
           glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
           glVertexAttribPointer(
               vertexUVID,                   // The attribute we want to configure
               2,                            // size : U+V => 2
               GL_FLOAT,                     // type
               GL_FALSE,                     // normalized?
               0,                            // stride
               (void*)0                      // array buffer offset
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
   glDeleteBuffers(1, &uvbuffer);

   // Exit program
   exit( EXIT_SUCCESS );
}

