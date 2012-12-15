#ifndef ROOT_HPP
#define ROOT_HPP

class Camera;
class Renderer;

class Root {
public:
    static Root& Instance();
    Camera*const  GetCamera();

    void Init();
    void Terminate();
    void Update();
    bool IsRunning();

private:
    Root();
    ~Root();

private:
    Camera* camera;
    Renderer* renderer;
    int running;

    // Performance counter
    int framesCounter;
    double framesDuration;
};
#endif
