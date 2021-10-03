#include "Camera.h"
#include "Frustum.h"


Camera::Camera(vec3 postion_, float pitch_, float yaw_, float aspect_, float fov_y_, float near_, float far_, float move_speed_, float roation_speed_)
    :pitch(pitch_), yaw(yaw_), aspect(aspect_), fov_y(glm::radians(fov_y_)), near_plane(near_), far_plane(far_), move_speed(move_speed_), rotation_speed(roation_speed_)
{
    transform.SetPosition(postion_);
    transform.SetPitch(pitch);
    transform.SetYaw(yaw);
    frustum = std::make_unique<Frustum>();
    
    move_speed = 8; //? 想办法一致
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
    frustum->Initialize(this);
}

glm::mat4 Camera::ViewMatrix()
{
    vec3 pos = transform.GetPosition();
    return lookAt(pos, pos + transform.GetForward(), transform.GetUpword());

    //vec3 forward;
    //forward.x = cos(yaw) * cos(pitch);
    //forward.y = sin(pitch);
    //forward.z = sin(yaw) * cos(pitch);
    //vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
    //vec3 up = normalize(cross(right, forward));
    //return lookAt(pos, pos + forward, up);
   
	//return  rotate(mat4(1), -pitch, { 1, 0, 0 }) * rotate(mat4(1), yaw, { 0, 1, 0 }) *
	//	translate(mat4(1), -pos);

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

void Camera::OnMouseMove(double x_offset, double y_offset)
{
    //cout << "Mouse Move pitch: " << pitch << " Yaw: " << yaw << endl;
	//transform.PrintMatrix();

    yaw -= x_offset * rotation_speed;   //？不知道为什么，通过欧拉角设置旋转矩阵，yaw的运动方向会反向
                                        // 即yaw是增大时，transform的yaw是减小
    pitch += y_offset * rotation_speed;



	pitch = std::max(pitch, -89.f);
	pitch = std::min(pitch, 89.f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
    transform.SetEulerAngle({ pitch, yaw, 0 }); 

	//cout << "View Matrix pitch: " << transform.GetPitch() << " Yaw: " << transform.GetYaw() << endl;
    //transform.SetEulerAngle({ transform.GetPitch() + y_offset * rotation_speed,
    //    transform.GetYaw() + x_offset * rotation_speed,
    //    0.f });

}

void Camera::OnKeyPressed(Window* window)
{
    GLFWwindow* glfw_window = window->GetGLFWWindow();
    float delta_time = window->GetDeltaTime();
	if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)  
		SetPosition(GetPosition() + GetForward() * delta_time * move_speed);
	if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
        SetPosition(GetPosition() - GetForward() * delta_time * move_speed);
	if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		SetPosition(GetPosition() - GetRight() * delta_time * move_speed);
	if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		SetPosition(GetPosition() + GetRight() * delta_time * move_speed);
}


