#include "BasicWindow.h"
#include "common/Camera.h"
#include <iostream>

BasicWindow::BasicWindow(GLFWwindow* glfw_window_) : Window(glfw_window_), first_mouse(true)
{

}


void BasicWindow::MouseCallback(double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos; // reversed since y-coordinates go from bottom to top

	last_x = x_pos;
	last_y = y_pos;

	CameraProcessMouseMovement(x_offset, y_offset);
}

void BasicWindow::ScrollCallback(double x_offset, double y_offset)
{
	CameraProcessMouseScroll(y_offset);
}

void BasicWindow::ProcessInput()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfw_window, true);
	CameraProcessKeyboard();
}

void BasicWindow::CameraProcessKeyboard()
{
	Transform tf = camera->GetTransform();
	if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
		camera->SetTransform(tf.Translate(tf.GetForward() * delta_time));
	if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
		camera->SetTransform(tf.Translate(tf.GetForward()* -delta_time));
	if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		camera->SetTransform(tf.Translate(tf.GetRight() * -delta_time));
	if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		camera->SetTransform(tf.Translate(tf.GetRight() * delta_time));
}

void BasicWindow::CameraProcessMouseMovement(float x_offset, float y_offset)
{
	if (camera) {
		Transform transform = camera->GetTransform();
		float yaw = transform.GetYaw();
		float pitch = transform.GetPitch();

		float rotation_speed = camera->GetRotationSpeed();
		yaw += x_offset * rotation_speed;
		pitch = y_offset * rotation_speed;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitch > 89.f)
			pitch = 89.f;
		if (pitch < -89.f)
			pitch = -89.f;

		transform.SetEulerAngle(vec3(pitch, yaw, transform.GetRoll()));

		camera->SetTransform(transform);
	}
	else
		std::cout << "ERROR<Window> from CameraProcessMouseMovement: camera is null \n";
}

void BasicWindow::CameraProcessMouseScroll(float y_offset)
{
	if (camera)
	{

	}
	else
		std::cout << "ERROR<Window> from CameraProcessMouseScroll: camera is null \n";
}
