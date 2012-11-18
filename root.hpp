#ifndef ROOT_HPP
#define ROOT_HPP

#include "renderer.hpp"

class Root {
public:
    Root();
    ~Root();

    void Init();
    void Terminate();
    void Update();
    bool IsRunning();

private:
    Renderer renderer;
    int running;
};

#endif