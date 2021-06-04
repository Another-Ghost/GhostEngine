#pragma once
#include "Prerequisite.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>



class Window	//? 改为全局
{

public:

	//Window(GLFWwindow* win, int width_, int height_) :glfw_window(win), width(width_), height(height_), first_mouse(true), last_x(width / 2.0f), last_y(height / 2.0f), delta_time(0.f){}
	Window(GLFWwindow* glfw_window_);

	//GLFWwindow* GetGLFWWindow() { return glfw_window; }
	//virtual bool Initialize();

	//子类重写后需先调用父类的
	virtual bool Update(float& dt);	

	virtual void EndUpdate();

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	float GetCurrentFrameTime() { return glfwGetTime(); }

	bool WindowShouldClose() { return glfwWindowShouldClose(glfw_window); }

protected:
	GLFWwindow* glfw_window;

	float delta_time;

	float last_frame_time;


	int width; //1280, 1920
	int height; //720, 1080

	virtual void FrameBufferSizeCallBack();	//可以把所有CallBack移出单独写个类
	virtual void MouseCallback(double x_pos, double y_pos);
	virtual void ScrollCallback(double x_offset, double y_offset);
	virtual void ProcessInput();

	friend class WindowManager;

};

class WindowEventListener
{
	virtual bool WindowClosed(Window* window);
};