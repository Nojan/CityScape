#include "renderable.hpp"

#include "camera.hpp"
#include "renderer.hpp"
#include "root.hpp"
#include "shader.hpp"

#include "objloader.hpp"
#include "texture.hpp"

#include "glm/gtc/type_ptr.hpp"

using namespace std;

RenderableInstance::RenderableInstance()
: mBind(false)
, mProgramShader(NULL)
{}

RenderableInstance::~RenderableInstance()
{
    assert(false == mBind);
}

void RenderableInstance::Init(ShaderProgram * programShader)
{
    mProgramShader = programShader;
}

GLuint RenderableInstance::ProgramID() const
{
    assert(NULL != mProgramShader);
    return mProgramShader->ProgramID();
}

const ShaderProgram * RenderableInstance::GetShaderProgram() const
{
    return mProgramShader;
}

void RenderableInstance::Bind()
{
    assert(false == mBind);
    assert(NULL != mProgramShader);
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

void RenderableInstance::UseShaderProgramIFN() const
{
    assert(NULL != mProgramShader);
    mProgramShader->UseShaderProgramIFN(); CHECK_OPENGL_ERROR
}

Renderable::Renderable()
: mInstance(0)
{}

Renderable::~Renderable()
{}

void Renderable::Init(glm::mat4 const & modelTransformMatrix, RenderableInstance const* instance)
{
    assert(NULL != instance);
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
    mInstance->UseShaderProgramIFN(); CHECK_OPENGL_ERROR
    mInstance->Draw(mModel); CHECK_OPENGL_ERROR
}

const RenderableInstance * Renderable::Instance() const
{
    return mInstance;
}

bool RenderableSorter(const Renderable *lhs, const Renderable *rhs)
{
    assert(NULL != lhs);
    assert(NULL != rhs);
    assert(NULL != lhs->Instance());
    assert(NULL != rhs->Instance());
    return lhs->Instance()->ProgramID() < rhs->Instance()->ProgramID();
}
