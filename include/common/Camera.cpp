#include "Camera.h"

Camera::~Camera()
{

}

glm::mat4 Camera::ViewMatrix()
{
    vec3 pos = transform.GetPosition();
    vec3 forward = transform.GetForward();
    vec3 right = transform.GetRight();
    vec3 up = transform.GetUpword();
    return lookAt(pos, pos + transform.GetForward(), transform.GetUpword());
}

glm::mat4 Camera::PerspectiveMatrix()
{
    return glm::perspective(fov_y, aspect, near_plane, far_plane);
}

void Camera::SetPitch(float pitch_)
{
    pitch = pitch_;
    transform.SetPitch(pitch);
}

void Camera::SetYaw(float yaw_)
{
    yaw = yaw_;
    transform.SetYaw(yaw);
}

void Camera::SetEulerAngle(const vec3& euler)
{
    pitch = euler.x;
    yaw = euler.y;
    transform.SetEulerAngle(euler);
}