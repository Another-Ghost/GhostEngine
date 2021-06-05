#pragma once
#include "common/Window.h"


class BasicWindow : public Window
{
public:
	BasicWindow(GLFWwindow* glfw_window_);

	void SetCamera(Camera* camera_) { camera = camera_; }
	const Camera& GetCamera() { return *camera; }

private:
	Camera* camera;

	bool first_mouse;

	float last_x;
	float last_y;


	//virtual void FrameBufferSizeCallBack() override;	//可以把所有CallBack移出单独写个类
	virtual void MouseCallback(double x_pos, double y_pos) override;
	virtual void ScrollCallback(double x_offset, double y_offset) override;
	virtual void ProcessInput() override;

	void CameraProcessKeyboard();
	void CameraProcessMouseMovement(float x_offset, float y_offset);
	void CameraProcessMouseScroll(float y_offset);

};

