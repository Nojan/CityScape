#ifndef RENDERABLE_TEXTURE_HPP
#define RENDERABLE_TEXTURE_HPP

#include "renderable.hpp"

class Texture2D;

class RenderableTextureInstance : public RenderableInstance
{
public:
    static RenderableTextureInstance * MakeInstanceFrom(char const * pathToObj, char const * pathToTexture);

    RenderableTextureInstance();
    ~RenderableTextureInstance();

    virtual void Draw(const glm::mat4 &model) const;

    virtual void Bind();
    virtual void Unbind();

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
};

#endif
