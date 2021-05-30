#include "BasicWindow.h"
#include "common/Camera.h"
#include <iostream>

BasicWindow::BasicWindow(GLFWwindow* glfw_window_) : Window(glfw_window_)
{

}

void BasicWindow::FrameBufferSizeCallBack()
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

	if (camera)
		camera->ProcessMouseMovement(x_offset, y_offset);
	else
		std::cout << "WARNING in Window: camera is null \n";
}

void BasicWindow::ScrollCallback(double x_offset, double y_offset)
{
	if (camera)
		camera->ProcessMouseScroll(y_offset);
	else
		std::cout << "WARNING in Window: camera is null \n";
}

void BasicWindow::ProcessInput()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfw_window, true);

	if (glfwGetKey(glfw_window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraControl::FORWARD, delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraControl::BACKWARD, delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraControl::LEFT, delta_time);
	if (glfwGetKey(glfw_window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraControl::RIGHT, delta_time);
}
