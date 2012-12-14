#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <vector>

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix, const char * pathToObj, const char * pathToTexture);
    void Terminate();
    void Draw(GLuint programID);
    void DrawDebug(GLuint programID);

private:
    GLuint vertexbuffer;
    GLuint vertexNormalbuffer;
    GLuint texture;
    GLuint uvbuffer;

    glm::mat4 model;

    std::vector<glm::vec3> vertexPosition;
    std::vector<glm::vec3> vertexNormal;
    std::vector<glm::vec2> uv;
};

#endif
