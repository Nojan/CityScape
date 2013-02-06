#include "renderer.hpp"

#include "camera.hpp"
#include "root.hpp"
#include "shader.hpp"
#include "skybox.hpp"

#include "buildingGenerator.hpp"
#include "skyboxGenerator.hpp"
#include "streetGenerator.hpp"

#include <algorithm>
#include <iostream>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

using namespace std;

Renderer::Renderer()
: mTextureProgram(NULL)
, mMaterialProgram(NULL)
, mSkyboxProgram(NULL)
, mSkybox(NULL)
{}

Renderer::~Renderer()
{}

void Renderer::Init()
{
    // Create and compile our GLSL program from the shaders
    GLuint loadedShaderID = 0;
    loadedShaderID = LoadShaders( "../shader/SimpleVertexShader.vertexshader", "../shader/SimpleFragmentShader.fragmentshader" );
    mTextureProgram = new TextureShaderProgram(loadedShaderID);
    loadedShaderID = LoadShaders( "../shader/MaterialVertex.shader", "../shader/MaterialFragment.shader" );
    mMaterialProgram = new MaterialShaderProgram(loadedShaderID);
    loadedShaderID  = LoadShaders( "../shader/Skybox.vertexshader", "../shader/Skybox.fragmentshader" );
    mSkyboxProgram = new ShaderProgram(loadedShaderID);
    cout << "Shader loaded." << endl;

    // OpenGL Setting
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    const unsigned int sceneRootSize = 50;

    Street_Generator::GenerateStreetScene(sceneRootSize, sceneRootSize, mBuildingInstanceList, mFloorInstanceList, mScene);
    assert( mScene.size() == sceneRootSize*sceneRootSize );

    for(size_t i=0; i<mBuildingInstanceList.size(); ++i)
    {
        RenderableInstance * instance = mBuildingInstanceList[i];
        instance->Init(mTextureProgram);
        instance->Bind();
    }

    for(size_t i=0; i<mFloorInstanceList.size(); ++i)
    {
        RenderableInstance * instance = mFloorInstanceList[i];
        instance->Init(mMaterialProgram);
        instance->Bind();
    }

    //Setup skybox
    mSkybox = Building_Generator::GenerateSkybox();

    // Setup Projection and Camera matrix
    Camera *const camera = Root::Instance().GetCamera();
    camera->SetPosition(glm::vec3(0,25,0));
    const float middleScene = 0.5f*sceneRootSize*1.f;
    camera->SetDirection(glm::normalize(glm::vec3(middleScene,0,middleScene)));
    camera->SetUp(glm::vec3(0,1,0));

    sort(mScene.begin(), mScene.end(), RenderableSorter);
}

void Renderer::Terminate()
{
    for(size_t i=0; i<mScene.size(); ++i)
        mScene[i]->Terminate();
    for(size_t i=0; i<mBuildingInstanceList.size(); ++i)
        mBuildingInstanceList[i]->Unbind();
    delete mSkybox;
    delete mSkyboxProgram;
    delete mTextureProgram;
    delete mMaterialProgram;
}

void Renderer::Update()
{
    // OpenGL rendering goes here...
    glClearDepth(1.0f); CHECK_OPENGL_ERROR
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); CHECK_OPENGL_ERROR

    glDisable(GL_DEPTH_TEST); CHECK_OPENGL_ERROR
    mSkybox->Draw(mSkyboxProgram);
    glEnable(GL_DEPTH_TEST); CHECK_OPENGL_ERROR
    glDepthFunc(GL_LESS); CHECK_OPENGL_ERROR
    //glEnable(GL_CULL_FACE); CHECK_OPENGL_ERROR

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
    for(size_t i=0; i<mScene.size(); ++i)
    {
        CHECK_OPENGL_ERROR
        mScene[i]->Draw();
        CHECK_OPENGL_ERROR
    }

    // Swap front and back rendering buffers
    glfwSwapBuffers(); CHECK_OPENGL_ERROR
}

void CheckOpenGLError()
{
    GLenum error;
    while ( (error = glGetError()) != GL_NO_ERROR)
    {
        printf( "OpenGL ERROR: %s\nCHECK POINT: %s (line %d)\n", gluErrorString(error), __FILE__, __LINE__ );
        assert(error == GL_NO_ERROR);
    }
}
