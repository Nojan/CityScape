#include "root.hpp"

#include "camera.hpp"
#include "renderer.hpp"

#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

using namespace std;

//Called when the window is resized
void GLFWCALL handleWindowResize(int width, int height)
{
    //Tell OpenGL how to convert from coordinates to pixel values
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION ); //Switch to setting the camera perspective
    const float ratio = static_cast<float>(width)/static_cast<float>(height);
    //Set the camera perspective
    glLoadIdentity(); //reset the camera
    gluPerspective( 60.0f, ratio, 1.0f, 100.0f );
}

//Called when a key state changed
void GLFWCALL handleKeyboardEvent(int key, int state)
{
    Root::Instance().GetCamera()->HandleKeyboardEvent(key, state);
}

//Called when the mouse move over the window
void GLFWCALL handleMousePosition(int x, int y)
{
    Root::Instance().GetCamera()->HandleMousePosition(x, y);
}

//Called when a mouse button state changed
void GLFWCALL handleMouseButton(int button, int state)
{
    Root::Instance().GetCamera()->HandleMouseButton(button, state);
}

//Called when the mouse move over the window
void GLFWCALL handleMouseWheel(int wheel)
{
    Root::Instance().GetCamera()->HandleMouseWheel(wheel);
}

Root& Root::Instance()
{
    static Root instance;
    return instance;
}

Root::Root()
: running(GL_FALSE)
{
    camera = new Camera();
    renderer = new Renderer();
}

Root::~Root()
{
    delete renderer;
    delete camera;
}

void Root::Init()
{
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
    const int defaultWidth = 800;
    const int defaultHeight = 600;
    if( !glfwOpenWindow( defaultWidth, defaultHeight, 0,0,0, 0,0,0, GLFW_WINDOW ) )
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
    glfwSetWindowSizeCallback(handleWindowResize);
    glfwSetCharCallback(handleKeyboardEvent);
    glfwSetMousePosCallback(handleMousePosition);
    glfwSetMouseButtonCallback(handleMouseButton);
    glfwSetMouseWheelCallback(handleMouseWheel);

    running = GL_TRUE;

    renderer->Init();
}

void Root::Terminate()
{
    renderer->Terminate();

    glfwTerminate();
}

void Root::Update()
{
    running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );

    renderer->Update();
}

bool Root::IsRunning()
{
    return running;
}

Camera *const Root::GetCamera()
{
    return camera;
}
