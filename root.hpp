#ifndef ROOT_HPP
#define ROOT_HPP

class Renderer;

class Root {
public:
    static Root& Instance();

    void Init();
    void Terminate();
    void Update();
    bool IsRunning();

private:
    Root();
    ~Root();

private:
    Renderer* renderer;
    int running;
};
#endif
