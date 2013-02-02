#include "renderable.hpp"

#include "camera.hpp"
#include "root.hpp"

#include "objloader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

using namespace std;

RenderableInstance::RenderableInstance()
: mBind(false)
, mProgramID(0)
{}

RenderableInstance::~RenderableInstance()
{
    assert(false == mBind);
}

void RenderableInstance::Init(GLuint programID)
{
    mProgramID = programID;
}

GLuint RenderableInstance::ProgramID() const
{
    return mProgramID;
}

void RenderableInstance::Bind()
{
    assert(false == mBind);

    mBind = true;
}

void RenderableInstance::Unbind()
{
    assert(true == mBind);

    mBind = false;
}

bool RenderableInstance::IsBind() const
{
    return mBind;
}

Renderable::Renderable()
: mInstance(0)
{}

Renderable::~Renderable()
{}

void Renderable::Init(glm::mat4 const & modelTransformMatrix, RenderableInstance const* instance)
{
    assert(instance);
    mModel = modelTransformMatrix;
    mInstance = instance;
}

void Renderable::Terminate()
{
    mInstance = 0;
}

void Renderable::Draw()
{
    assert(mInstance);

    mInstance->Draw(mModel);
}

const RenderableInstance * Renderable::Instance() const
{
    return mInstance;
}
