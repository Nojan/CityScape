#include "renderer.hpp"

#include "camera.hpp"
#include "root.hpp"
#include "shader.hpp"

#include <iostream>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

using namespace std;

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::Init()
{
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "../shader/SimpleVertexShader.vertexshader", "../shader/SimpleFragmentShader.fragmentshader" );
    programDebugID = LoadShaders( "../shader/DebugVertexShader.vertexshader", "../shader/DebugFragmentShader.fragmentshader" );
    cout << "Shader loaded." << endl;

    // Setup Projection and Camera matrix
    Camera *const camera = Root::Instance().GetCamera();
    camera->SetPosition(glm::vec3(10,0,0));
    camera->SetDirection(glm::normalize(glm::vec3(10,0,0)*-1.f));
    camera->SetUp(glm::vec3(0,1,0));

    // OpenGL Setting
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);

    renderable1.Init(glm::mat4(1.f), "../asset/cube.obj", "../asset/uvtemplate.bmp");
    renderable2.Init( glm::translate(glm::mat4(1.f), glm::vec3(-3.f,0.f,2.f)), "../asset/cube.obj", "../asset/uvtemplate.bmp" );
}

void Renderer::Terminate()
{
    renderable1.Terminate();
    renderable2.Terminate();
    glDeleteProgram(programID);
    glDeleteProgram(programDebugID);
}

void Renderer::Update()
{
    // OpenGL rendering goes here...
    glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // draw debug
    if(false)
    {
        glUseProgram(programDebugID);
        glPointSize(1.f);
        glLineWidth(2.f);
        renderable1.DrawDebug(programDebugID);
        renderable2.DrawDebug(programDebugID);
    }
    //lighting
    {
        GLfloat DiffuseLight[] = {0.f, 0.f, 1.f};
        GLfloat AmbientLight[] = {0.1f, 0.1f, 0.1f};
        GLfloat LightPosition[] = {5.f, 5.f, 5.f, 0.f};

        glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
        glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight);
        glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
    }

    // Rendering
    glUseProgram(programID);
    renderable1.Draw(programID);
    renderable2.Draw(programID);
    glUseProgram(0);

    // Swap front and back rendering buffers
    glfwSwapBuffers();
}
