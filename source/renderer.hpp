#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderable.hpp"

class Skybox;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Terminate();
    void Update();

private:
    GLuint mProgramID;
    GLuint mProgramDebugID;
    GLuint mSkyboxShaderID;
    Skybox * mSkybox;
    std::vector<RenderableInstance*> mRenderableInstanceList;
    std::vector<Renderable> mScene;
};

#endif
