#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "Window.h"
#include "WindowFactory.h"
#include <vector>
#include <string>


class WindowManager : public Singleton<WindowManager>
{
public:
	static vector<Window*> window_array;	//? 改成监听/观察者列表

	static Window* s_current_window;	//因为callback函数为static，只能获取static成员变量

	bool b_initialized;

	WindowManager(int width, int height, const string& title);

	bool Initialize();

	void Update(float dt);

	void EndUpdate(float dt);

	void Terminate();

	Window* CreateWindow(int width, int height, const std::string& title);


private:
	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
	static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);


};

