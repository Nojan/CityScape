#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

#include "shader.hpp"
#include "texture.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Terminate();
    void Update();

private:
    GLuint vertexbuffer;
    GLuint texture;
    GLuint uvbuffer;
    GLuint programID;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif
