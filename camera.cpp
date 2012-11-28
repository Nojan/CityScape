#include "camera.hpp"

#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glfw.h>

using namespace std;

Camera::Camera()
{}

Camera::~Camera()
{}

void Camera::Init()
{

}

void Camera::Terminate()
{

}

void Camera::Update()
{

}

void Camera::HandleKeyboardEvent(int key, int state)
{
    cout << "Keyboard " << key << " " << state << endl;
}

void Camera::HandleMousePosition(int x, int y)
{
    cout << "Mouse (" << x << "," << y << ")" << endl;
}

void Camera::HandleMouseButton(int button, int state)
{
    cout << "Mouse Button " << button << " " << state << endl;
}

void Camera::HandleMouseWheel(int wheel)
{
    cout << "Mouse wheel " << wheel << endl;
}

