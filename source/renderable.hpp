#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <vector>

class Texture2D;

class RenderableInstance {
public:
    static RenderableInstance * MakeInstanceFrom(char const * pathToObj, char const * pathToTexture);

    RenderableInstance();
    ~RenderableInstance();

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
    GLuint indexbuffer;
    GLuint vertexbuffer;
    GLuint vertexNormalbuffer;
    GLuint uvbuffer;
    GLuint texturebuffer;

    bool bind;
};

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix, RenderableInstance const* instance);
    void Terminate();
    void Draw(GLuint programID);
    void DrawDebug(GLuint programID);

private:
    glm::mat4 mModel;

    RenderableInstance const* mInstance;
};

#endif
