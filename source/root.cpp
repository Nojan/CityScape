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
    Root::Instance().GetCamera()->HandleWindowResize(width, height);
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
: mRunning(GL_FALSE)
, mFramesCounter(0)
, mFrameDuration(0)
, mFramesDuration(0)
{
    mCamera = new Camera();
    mRenderer = new Renderer();
}

Root::~Root()
{
    delete mRenderer;
    delete mCamera;
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
    const int defaultRed = 0;
    const int defaultGreen = 0;
    const int defaultBlue = 0;
    const int defaultAlpha = 0;
    const int defaultDepth = 8;
    const int defaultStencil = 0;
    if( !glfwOpenWindow( defaultWidth, defaultHeight, defaultRed,defaultGreen,defaultBlue,defaultAlpha, defaultDepth, defaultStencil, GLFW_WINDOW ) )
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
    glfwEnable( GLFW_STICKY_KEYS );
    glfwSetWindowSizeCallback(handleWindowResize);
    glfwSetMousePosCallback(handleMousePosition);
    glfwSetMouseButtonCallback(handleMouseButton);
    glfwSetMouseWheelCallback(handleMouseWheel);

    mRunning = GL_TRUE;

    mCamera->Init();
    mRenderer->Init();
}

void Root::Terminate()
{
    mCamera->Terminate();
    mRenderer->Terminate();

    glfwTerminate();
}

void Root::Update()
{
    const double frameLimiter = 1/60;
    mRunning = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );

    glfwSetTime(0);
    mCamera->Update(mFrameDuration);
    mRenderer->Update();
    mFrameDuration = glfwGetTime();

    ++mFramesCounter;
    mFramesDuration += mFrameDuration;
    glfwSleep( frameLimiter - mFrameDuration);
    if(mFramesCounter > 100)
    {
        const double avgFrameDuration = mFramesDuration / static_cast<double>(mFramesCounter);
        std::cout << "Average frame : " << avgFrameDuration << "s" << std::endl;
    }
}

bool Root::IsRunning()
{
    return mRunning;
}

Camera *const Root::GetCamera()
{
    return mCamera;
}
