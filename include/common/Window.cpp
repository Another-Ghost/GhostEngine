#include "Window.h"
#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;


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

	camera.ProcessMouseMovement(x_offset, y_offset);
}

void Window::ScrollCallback(double x_offset, double y_offset)
{
	camera.ProcessMouseScroll(y_offset);
}

void Window::ProcessInput()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfw_window, true);
}


