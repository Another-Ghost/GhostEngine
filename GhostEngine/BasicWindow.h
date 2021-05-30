#pragma once
#include "common/Window.h"

class Camera;

class BasicWindow : public Window
{
public:
	BasicWindow(GLFWwindow* glfw_window_);

private:
	Camera* camera;

	bool first_mouse;

	float last_x;
	float last_y;


	virtual void FrameBufferSizeCallBack() override;	//可以把所有CallBack移出单独写个类
	virtual void MouseCallback(double x_pos, double y_pos) override;
	virtual void ScrollCallback(double x_offset, double y_offset) override;
	virtual void ProcessInput() override;


};

