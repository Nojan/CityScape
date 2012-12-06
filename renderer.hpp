#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderable.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Terminate();
    void Update();

private:
    GLuint programID;
    Renderable renderable1;
    Renderable renderable2;
};

#endif
