#include "camera.hpp"

#include <glm/gtx/transform.hpp>

#include <iostream>

using namespace std;

#define MV_NONE  0b00000000
#define MV_LEFT  0b00000001
#define MV_RIGHT 0b00000010
#define MV_UP    0b00000100
#define MV_DOWN  0b00001000

Camera::Camera()
: mMoveMask(MV_NONE)
, mUpdateView(true)
, mUpdateProjection(true)
, mMousePan(false)
, mLastWheel(0)
, mMousePosition(0.f)
, mPosition(1.f, 0, 0)
, mDirection(0, 0, 1.f)
, mUp(0, 1.f, 0)
, mOrthoDirection(glm::cross(mDirection, mUp))
{
    mPerspective.fov = 60.f;
    mPerspective.ratio = 4.f/3.f;
    mPerspective.zNear = 1.f;
    mPerspective.zFar = 100.f;
}

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
    HandleKeyboardEvent();
    if( MV_NONE != mMoveMask )
    {
        const float moveSpeed = 0.1f;
        if(MV_LEFT & mMoveMask)
            mPosition -= mOrthoDirection*moveSpeed;
        if(MV_RIGHT & mMoveMask)
            mPosition += mOrthoDirection*moveSpeed;
        if(MV_UP & mMoveMask)
            mPosition += mDirection*moveSpeed;
        if(MV_DOWN & mMoveMask)
            mPosition -= mDirection*moveSpeed;
        mUpdateView = true;
        cout << "[" << mPosition.x << "," << mPosition.y << "," << mPosition.z << "]" << endl;
    }

    if(mUpdateView)
        mView = glm::lookAt(mPosition, mPosition+mDirection, mUp);
    if(mUpdateProjection)
        mProjection = glm::perspective(mPerspective.fov, mPerspective.ratio, mPerspective.zNear, mPerspective.zFar);
    if(mUpdateView || mUpdateProjection)
    {
        mProjectionView = mProjection*mView;
        mUpdateView = false;
        mUpdateProjection = false;
    }
}

Camera::perspective const& Camera::Perspective() const
{
    return mPerspective;
}

glm::vec3 const& Camera::Position() const
{
    return mPosition;
}

void Camera::SetPosition(glm::vec3 const& position)
{
    mPosition = position;
    mUpdateView = true;
}

glm::vec3 const& Camera::Direction() const
{
    return mDirection;
}

void Camera::SetDirection(glm::vec3 const& direction)
{
    mDirection = direction;
    mOrthoDirection = glm::cross(mDirection, mUp);
    mUpdateView = true;
}

glm::vec3 const& Camera::Up() const
{
    return mUp;
}

void Camera::SetUp(glm::vec3 const& up)
{
    mUp = up;
    mOrthoDirection = glm::cross(mDirection, mUp);
    mUpdateView = true;
}

glm::mat4 const& Camera::View() const
{
    return mView;
}

glm::mat4 const& Camera::Projection() const
{
    return mProjection;
}

glm::mat4 const& Camera::ProjectionView() const
{
    return mProjectionView;
}

void Camera::HandleWindowResize(int width, int height)
{
    mPerspective.ratio = static_cast<float>(width)/static_cast<float>(height);
    mUpdateProjection = true;
}

void Camera::HandleKeyboardEvent()
{
    if( glfwGetKey(GLFW_KEY_DOWN) )
        mMoveMask |= MV_DOWN;
    else
        mMoveMask &= ~MV_DOWN;

    if( glfwGetKey(GLFW_KEY_LEFT) )
        mMoveMask |= MV_LEFT;
    else
        mMoveMask &= ~MV_LEFT;

    if( glfwGetKey(GLFW_KEY_UP) )
        mMoveMask |= MV_UP;
    else
        mMoveMask &= ~MV_UP;

    if( glfwGetKey(GLFW_KEY_RIGHT) )
        mMoveMask |= MV_RIGHT;
    else
        mMoveMask &= ~MV_RIGHT;
}

void Camera::HandleMousePosition(int x, int y)
{
    const glm::vec2 newMousePosition(x, y);
    if(mMousePan)
    {
        const float gain = 0.1f;
        const glm::vec2 vec = (newMousePosition-mMousePosition)*gain;

        const glm::mat3 pitch = glm::mat3(glm::rotate(vec.x, mUp));
        const glm::mat3 yaw = glm::mat3(glm::rotate(vec.y, mOrthoDirection));

        mDirection = yaw * pitch * mDirection;
        mUp = pitch * mUp;
        mOrthoDirection = glm::cross(mDirection, mUp);
        mUpdateView = true;
    }
    mMousePosition = newMousePosition;
}

void Camera::HandleMouseButton(int button, int state)
{
    mMousePan = (button == GLFW_MOUSE_BUTTON_LEFT) && (state == GLFW_PRESS);
}

void Camera::HandleMouseWheel(int wheel)
{
    if (wheel < mLastWheel)
        ++mPerspective.fov;
    else
        --mPerspective.fov;
    mLastWheel = wheel;
    mUpdateProjection = true;
}

