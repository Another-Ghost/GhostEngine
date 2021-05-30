#include "Camera.h"

void Camera::ProcessKeyboard(CameraControl type, float dt)
{
    float velocity = move_speed * dt;
    if (type == CameraControl::FORWARD)
        transform.SetPosition(transform.GetPosition() + transform.GetForward() * dt);
	if (type == CameraControl::BACKWARD)
		transform.SetPosition(transform.GetPosition() - transform.GetForward() * dt);
	if (type == CameraControl::RIGHT)
		transform.SetPosition(transform.GetPosition() + transform.GetRight() * dt);
	if (type == CameraControl::LEFT)
		transform.SetPosition(transform.GetPosition() - transform.GetRight() * dt);
}

void Camera::ProcessMouseMovement(float x_offset, float y_offset)
{
    float yaw = transform.GetYaw();
    float pitch = transform.GetPitch();

    yaw += x_offset * rotation_spead;
    pitch = y_offset * rotation_spead;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    transform.SetEulerAngle(vec3(pitch, yaw, transform.GetRoll()));
}

void Camera::ProcessMouseScroll(float y_offset)
{
}

glm::mat4 Camera::ViewMatrix()
{
    vec3 pos = transform.GetPosition();
    return lookAt(pos, pos + transform.GetForward(), transform.GetUpword());
}

glm::mat4 Camera::PerspectiveMatrix()
{
    return glm::perspective(fov_y, aspect, near_plane, far_plane);
}
