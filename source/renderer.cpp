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

    renderableInstance = RenderableInstance::MakeInstanceFrom("../asset/cube.obj", "../asset/uvtemplate.bmp");
    renderableInstance->Bind();
    scene.resize(20);
    glm::mat4 identity(1.f);
    for(size_t i=0; i<scene.size(); ++i)
    {
        identity = glm::translate(identity, glm::vec3(0.f,0.f,4.f));
        scene[i].Init(identity, renderableInstance);
    }
}

void Renderer::Terminate()
{
    for(size_t i=0; i<scene.size(); ++i)
        scene[i].Terminate();
    glDeleteProgram(programID);
    glDeleteProgram(programDebugID);
    renderableInstance->Unbind();
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
        for(size_t i=0; i<scene.size(); ++i)
            scene[i].DrawDebug(programDebugID);
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
    for(size_t i=0; i<scene.size(); ++i)
        scene[i].Draw(programID);
    glUseProgram(0);

    // Swap front and back rendering buffers
    glfwSwapBuffers();
}
