#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

class Camera {
public:
    Camera();
    ~Camera();

    void Init();
    void Terminate();
    void Update();

    void HandleKeyboardEvent(int key, int state);
    void HandleMousePosition(int x, int y);
    void HandleMouseButton(int button, int state);
    void HandleMouseWheel(int wheel);

private:

};

#endif
