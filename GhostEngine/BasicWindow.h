﻿#pragma once
#include "common/Window.h"


class BasicWindow : public Window
{
public:
	BasicWindow(GLFWwindow* glfw_window_);

	void SetCamera(Camera* camera_) { camera = camera_; }
	const Camera& GetCamera() { return *camera; }

private:
	Camera* camera;

	//virtual void FrameBufferSizeCallBack() override;	//可以把所有CallBack移出单独写个类
	virtual void ScrollCallback(double x_offset, double y_offset) override;
	virtual void ProcessInput() override;

	void CameraProcessKeyboard();
	void CameraProcessMouseMovement(double x_offset, double y_offset);
	void CameraProcessMouseScroll(float y_offset);

};

