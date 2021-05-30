#pragma once
#include "Singleton.h"
#include "Window.h"
#include "WindowFactory.h"
#include <vector>
#include <string>


class WindowManager : public Singleton<WindowManager>
{
public:
	static std::vector<Window*> window_array;

	static Window* current_window;

	bool b_initialized;

	bool Initialize();

	Window* CreateWindow(WindowFactory* factory, int width, int height, const std::string& title);

private:
	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
	static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);


};

