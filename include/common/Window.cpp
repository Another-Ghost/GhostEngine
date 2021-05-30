#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;


bool Window::Update(float& dt)
{
	if (!glfwWindowShouldClose(glfw_window))
	{
		float current_frame_time = glfwGetTime();
		delta_time = current_frame_time - last_frame_time;
		last_frame_time = current_frame_time;
		
		dt = delta_time;

		ProcessInput();
		return true;
	}
	else
	{
		return false;
	}
}

void Window::EndUpdate()
{
	glfwSwapBuffers(glfw_window);
	glfwPollEvents();
}

void Window::MouseCallback(double x_pos, double y_pos)
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

void Window::ScrollCallback(double x_offset, double y_offset)
{
	if(camera)
		camera->ProcessMouseScroll(y_offset);
	else
		std::cout << "WARNING in Window: camera is null \n";
}

void Window::ProcessInput()
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


