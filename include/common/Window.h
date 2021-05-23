#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"



class Window
{

public:

	const unsigned int kWidth; //1280, 1920
	const unsigned int kHeight; //720, 1080

public:

	Window(GLFWwindow* win, int width, int height) :glfw_window(win), kWidth(width), kHeight(height), first_mouse(true), last_x(width / 2.0f), last_y(height / 2.0f) {}

	//GLFWwindow* GetGLFWWindow() { return glfw_window; }

private:

	GLFWwindow* glfw_window;

	bool first_mouse;
	float last_x;
	float last_y;

	Camera camera;

private:
	void FrameBufferSizeCallBack();
	void MouseCallback(double x_pos, double y_pos);
	void ScrollCallback(double x_offset, double y_offset);
	void ProcessInput();

	friend class Initializer;
};

