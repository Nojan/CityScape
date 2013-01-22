#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderable.hpp"

class Skybox;
class RenderableMaterialInstance;

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
    RenderableMaterialInstance * mFloorInstance;
    std::vector<RenderableInstance*> mBuildingInstanceList;
    std::vector<Renderable> mScene;
};

#endif
