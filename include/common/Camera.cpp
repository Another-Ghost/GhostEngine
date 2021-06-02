#include "Camera.h"


glm::mat4 Camera::ViewMatrix()
{
    vec3 pos = transform.GetPosition();
    return lookAt(pos, pos + transform.GetForward(), transform.GetUpword());
}

glm::mat4 Camera::PerspectiveMatrix()
{
    return glm::perspective(fov_y, aspect, near_plane, far_plane);
}
