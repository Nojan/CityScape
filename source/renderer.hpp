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
    GLuint programID;
    GLuint programDebugID;
    GLuint mSkyboxShaderID;
    Skybox * mSkybox;
    std::vector<RenderableInstance*> mRenderableInstanceList;
    std::vector<Renderable> scene;
};

#endif
