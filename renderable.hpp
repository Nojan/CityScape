#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

class Renderable {
public:
    Renderable();
    ~Renderable();

    void Init(glm::mat4 const& modelTransformMatrix);
    void Terminate();
    void Draw(GLuint programID);

private:
    GLuint vertexbuffer;
    GLuint texture;
    GLuint uvbuffer;

    glm::mat4 model;
};

#endif
