#pragma once
#include "Window.h"

class GLFWwindow;

class WindowFactory
{
public:
	virtual Window* CreateWindow(GLFWwindow* glfw_window) = 0;

private:
	virtual ~WindowFactory(){}	//多态基类需声明virtual析构函数，否则由base指针删除时会错误的只调用基类的析构函数

};

