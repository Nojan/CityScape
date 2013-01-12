#include "renderer.hpp"

#include "camera.hpp"
#include "root.hpp"
#include "shader.hpp"
#include "skybox.hpp"

#include "buildingGenerator.hpp"
#include "skyboxGenerator.hpp"

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

    // OpenGL Setting
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    const size_t variation = 10;
    for(size_t i=0; i<variation; ++i)
    {
        unsigned int width = 6+rand()%4;
        unsigned int length = 6+rand()%4;
        unsigned int height = 6+rand()%20;

        RenderableInstance * renderableInstance = Building_Generator::GenerateBox(width, length, height);
        renderableInstance->Bind();

        mRenderableInstanceList.push_back(renderableInstance);
    }

    const size_t sceneRootSize = 10;
    const float spacing = 10.f;
    scene.resize(sceneRootSize*sceneRootSize);
    glm::mat4 matTransform(1.f);
    for(size_t i=0; i<sceneRootSize; ++i)
    {
        matTransform = glm::translate(glm::mat4(1.f), glm::vec3(i*spacing,0.f,0.f));
        const size_t iOffset = i*sceneRootSize;
        for(size_t j=0; j<sceneRootSize; ++j)
        {
            matTransform = glm::translate(matTransform, glm::vec3(0.f,0.f,spacing));
            scene[iOffset+j].Init(matTransform, mRenderableInstanceList[rand()%variation]);
        }
    }

    //Setup skybox
    mSkybox = Building_Generator::GenerateSkybox();
    mSkyboxShaderID  = LoadShaders( "../shader/Skybox.vertexshader", "../shader/Skybox.fragmentshader" );

    // Setup Projection and Camera matrix
    Camera *const camera = Root::Instance().GetCamera();
    camera->SetPosition(glm::vec3(0,25,0));
    const float middleScene = 0.5f*sceneRootSize*spacing;
    camera->SetDirection(glm::normalize(glm::vec3(middleScene,0,middleScene)));
    camera->SetUp(glm::vec3(0,1,0));
}

void Renderer::Terminate()
{
    for(size_t i=0; i<scene.size(); ++i)
        scene[i].Terminate();
    for(size_t i=0; i<mRenderableInstanceList.size(); ++i)
        mRenderableInstanceList[i]->Unbind();
    delete mSkybox;
    glDeleteProgram(mSkyboxShaderID);
    glDeleteProgram(programID);
    glDeleteProgram(programDebugID);
}

void Renderer::Update()
{
    // OpenGL rendering goes here...
    glClearDepth(1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable(GL_DEPTH_TEST);
    mSkybox->Draw(mSkyboxShaderID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);

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
        GLfloat DiffuseLight[] = {0.f, 0.f, 0.f};
        GLfloat AmbientLight[] = {1.f, 1.f, 1.f};
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
