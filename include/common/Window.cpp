#include "Window.h"
#include <iostream>
//#include <functional>

using namespace std;


Window::Window(GLFWwindow* glfw_window_): glfw_window(glfw_window_)
{
	glfwGetWindowSize(glfw_window_, &width, &height);
}

void Window::Update(float dt)
{
	delta_time = dt;
	ProcessInput();

}

void Window::EndUpdate()
{
	glfwSwapBuffers(glfw_window);
	glfwPollEvents();
}

void Window::FrameBufferSizeCallBack()
{

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


