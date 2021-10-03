#pragma once
#include "BasicDependencies.h"


class WindowEventListener;

class Window	//? 改为全局
{

public:

	//Window(GLFWwindow* win, int width_, int height_) :glfw_window(win), width(width_), height(height_), first_mouse(true), last_x(width / 2.0f), last_y(height / 2.0f), delta_time(0.f){}
	Window(GLFWwindow* glfw_window_);

	//GLFWwindow* GetGLFWWindow() { return glfw_window; }
	//virtual bool Initialize();

	//子类重写后需先调用父类的
	virtual void Update(float dt);	

	virtual void EndUpdate();

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	float GetDeltaTime() { return delta_time; }

	double GetCurrentFrameTime() { return glfwGetTime(); }

	GLFWwindow* GetGLFWWindow() { return glfw_window; }

	bool WindowShouldClose() { return glfwWindowShouldClose(glfw_window); }

	void AddEventListener(WindowEventListener* listener_) { listener_set.emplace(listener_); }
	void RemoveEventListener(WindowEventListener* listener_) { listener_set.erase(listener_); }

protected:
	GLFWwindow* glfw_window;

	float delta_time;

	float last_frame_time;


	int width; //1280, 1920
	int height; //720, 1080

	set<WindowEventListener*> listener_set;

	bool first_mouse;

	double last_x;
	double last_y;

	virtual void FrameBufferSizeCallBack();	//可以把所有CallBack移出单独写个类
	virtual void MouseCallback(double x_pos, double y_pos);
	virtual void ScrollCallback(double x_offset, double y_offset);
	virtual void ProcessInput();
			
	void ProcessListenerInput();

	friend class WindowManager;

};

class WindowEventListener
{
public:
	virtual void OnWindowClosed();
	virtual void OnFrameBufferSizeChanged();
	virtual void OnMouseMove(double x_offset, double y_offset);
	virtual void OnMouseScroll(double x_offset, double y_offset);
	virtual void OnKeyPressed(Window* window);
};