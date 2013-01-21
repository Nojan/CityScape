#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <vector>

class Texture2D;

class RenderableTextureInstance {
public:
    static RenderableTextureInstance * MakeInstanceFrom(char const * pathToObj, char const * pathToTexture);

    RenderableTextureInstance();
    ~RenderableTextureInstance();

    void Init(GLuint programID);

    void Draw(const glm::mat4 &model) const;

    void Bind();
    void Unbind();
    bool IsBind() const;

    GLuint IndexId() const;
    GLuint VertexId() const;
    GLuint NormalId() const;
    GLuint UvId() const;
    GLuint TextureId() const;

    std::vector<unsigned short> index;
    std::vector<glm::vec3> vertexPosition;
    std::vector<glm::vec3> vertexNormal;
    std::vector<glm::vec2> uv;
    Texture2D* texture;

private:
    GLuint mIndexbuffer;
    GLuint mVertexbuffer;
    GLuint mVertexNormalbuffer;
    GLuint mUvbuffer;
    GLuint mTexturebuffer;
    GLuint mProgramID;

    bool mBind;
};

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix, RenderableTextureInstance const* instance);
    void Terminate();
    void Draw();
    void DrawDebug(GLuint programID);

private:
    glm::mat4 mModel;

    RenderableTextureInstance const* mInstance;
};

#endif
