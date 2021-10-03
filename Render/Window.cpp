#include "Window.h"
#include <iostream>
//#include <functional>

using namespace std;


Window::Window(GLFWwindow* glfw_window_): glfw_window(glfw_window_),
	first_mouse(true)
{
	glfwGetWindowSize(glfw_window_, &width, &height);
}

void Window::Update(float dt)
{
	delta_time = dt;
	ProcessInput();
	ProcessListenerInput();
}

void Window::EndUpdate()
{
	glfwSwapBuffers(glfw_window);
	glfwPollEvents();	//处理pending的事件
}

void Window::FrameBufferSizeCallBack()
{
	
}

void Window::MouseCallback(double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	double x_offset = x_pos - last_x;
	double y_offset = last_y - y_pos; // reversed since y-coordinates go from bottom to top

	last_x = x_pos;
	last_y = y_pos;

	for (const auto& listener : listener_set)
	{
		if (listener)
		{
			listener->OnMouseMove(x_offset, y_offset);
		}
	}
}

void Window::ScrollCallback(double x_offset, double y_offset)
{

}

void Window::ProcessInput()
{
	if (glfwGetKey(glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfw_window, true);
}

void Window::ProcessListenerInput()
{
	for (const auto& listener : listener_set)
	{
		if (listener)
		{
			listener->OnKeyPressed(this);
		}
	}
}


void WindowEventListener::OnWindowClosed()
{

}

void WindowEventListener::OnFrameBufferSizeChanged()
{

}

void WindowEventListener::OnMouseMove(double x_pos, double y_pos)
{

}

void WindowEventListener::OnMouseScroll(double x_offset, double y_offset)
{

}

void WindowEventListener::OnKeyPressed(Window* window)
{

}
