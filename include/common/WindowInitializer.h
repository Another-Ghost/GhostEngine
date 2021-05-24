#pragma once

class Window;
class GLFWwindow;

class WindowInitializer
{


public:
	WindowInitializer();

	bool Init();

	Window* CreateWindow(Window* win, int width, int height);

	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
	static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);

private:
	Window* window;

};

