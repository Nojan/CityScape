#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <vector>

class Texture2D;
class ShaderProgram;

class RenderableInstance
{
public:
    RenderableInstance();
    virtual ~RenderableInstance();

    void Init(ShaderProgram * programShader);
    GLuint ProgramID() const;

    virtual void Draw(const glm::mat4 &model) const = 0;

    virtual void Bind();
    virtual void Unbind();
    bool IsBind() const;

    void UseShaderProgramIFN() const;

private:
    bool mBind;
    ShaderProgram * mProgramShader;
};

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix, RenderableInstance const* instance);
    void Terminate();
    void Draw();

    const RenderableInstance * Instance() const;

private:
    glm::mat4 mModel;

    RenderableInstance const* mInstance;
};

bool RenderableSorter(const Renderable * lhs, const Renderable * rhs);

#endif
