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
    GLuint programDebugID;
    Renderable::Data const * data;
    std::vector<Renderable> scene;
};

#endif
