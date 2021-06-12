#include "BasicWindow.h"
#include "common/Camera.h"
#include <iostream>

BasicWindow::BasicWindow(GLFWwindow* glfw_window_) : Window(glfw_window_)
{
	
}



void BasicWindow::ScrollCallback(double x_offset, double y_offset)
{
	CameraProcessMouseScroll(y_offset);
}

void BasicWindow::ProcessInput()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfw_window, true);
	//CameraProcessKeyboard();
}

void BasicWindow::CameraProcessKeyboard()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() + camera->GetForward() * delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() - camera->GetForward() * delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() - camera->GetRight() * delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() + camera->GetRight() * delta_time);
}

void BasicWindow::CameraProcessMouseMovement(double x_offset, double y_offset)
{
	if (camera) {
		float yaw = camera->GetYaw();
		float pitch = camera->GetPitch();

		//cout << "Pitch Previous:" << pitch << " ";
		//cout << "Yaw Previous:" << yaw << "\n";
		
		vec3 pos = camera->GetPosition();
		//cout << "Position：" << pos.x<<" " <<pos.y << " " << pos.z<<"\n";

		float rotation_speed = camera->GetRotationSpeed();
		yaw += x_offset * rotation_speed;
		pitch += y_offset * rotation_speed;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		//if (pitch > 89.f)
		//	pitch = 89.f;
		//if (pitch < -89.f)
		//	pitch = -89.f;

		pitch = std::max(pitch, -90.f);
		pitch = std::min(pitch, 90.f);

		if (yaw < 0) {
			yaw += 360.0f;
		}
		if (yaw > 360.0f) {
			yaw -= 360.0f;
		}

		camera->SetEulerAngle(vec3(pitch, yaw, 0));
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
