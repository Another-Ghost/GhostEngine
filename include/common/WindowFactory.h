#pragma once
#include "Window.h"

class WindowFactory
{
	virtual Window* CreateWindow() = 0;

	virtual ~WindowFactory(){}	//多态基类需声明virtual析构函数，否则由base指针删除时会错误的只调用基类的析构函数

};

