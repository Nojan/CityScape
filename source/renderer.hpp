#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderable.hpp"

class Skybox;
class ShaderProgram;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Terminate();
    void Update();

private:
    ShaderProgram * mTextureProgram;
    ShaderProgram * mMaterialProgram;
    ShaderProgram * mSkyboxProgram;
    Skybox * mSkybox;
    std::vector<RenderableInstance*> mFloorInstanceList;
    std::vector<RenderableInstance*> mBuildingInstanceList;
    std::vector<Renderable*> mScene;
};

void CheckOpenGLError();

#if 1
#define CHECK_OPENGL_ERROR
#else
#define CHECK_OPENGL_ERROR { CheckOpenGLError(); }
#endif

#endif
