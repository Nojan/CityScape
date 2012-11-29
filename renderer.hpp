#ifndef RENDERER_HPP
#define RENDERER_HPP

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
};

#endif
