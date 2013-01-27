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
    GLuint mTextureProgramID;
    GLuint mMaterialProgramID;
    GLuint mSkyboxProgramID;
    Skybox * mSkybox;
    std::vector<RenderableInstance*> mFloorInstanceList;
    std::vector<RenderableInstance*> mBuildingInstanceList;
    std::vector<Renderable> mScene;
};

#endif
