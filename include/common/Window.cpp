#include "Window.h"
#include <iostream>
//#include <functional>

using namespace std;


Window::Window(GLFWwindow* glfw_window_): glfw_window(glfw_window_)
{
	glfwGetWindowSize(glfw_window_, &width, &height);
}

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

}

void Window::ScrollCallback(double x_offset, double y_offset)
{

}

void Window::ProcessInput()
{

}


