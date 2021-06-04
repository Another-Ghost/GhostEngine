#pragma once
#include "Window.h"


class WindowFactory //因为需要声明生成不同种类的不同工厂，所以不能继承自Singleton
{
public:
	virtual Window* CreateWindow(GLFWwindow* glfw_window) = 0;

protected:
	virtual ~WindowFactory(){}	//多态基类需声明virtual析构函数，否则由base指针删除时会错误的只调用基类的析构函数

};

