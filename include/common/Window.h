#pragma once

#include "Camera.h"

struct GLFWwindow;

class Window	//? 改为全局
{

public:

	//Window(GLFWwindow* win, int width_, int height_) :glfw_window(win), width(width_), height(height_), first_mouse(true), last_x(width / 2.0f), last_y(height / 2.0f), delta_time(0.f){}

	//GLFWwindow* GetGLFWWindow() { return glfw_window; }
	virtual bool Initialize();

	virtual bool Update(float & dt);

	virtual void EndUpdate();

	//void SetCamera(Camera* cam) { camera = cam; }

protected:
	GLFWwindow* glfw_window;

	//Camera* camera;

	float delta_time;

private:
	unsigned int width; //1280, 1920
	unsigned int height; //720, 1080

	bool first_mouse;
	float last_x;
	float last_y;

	float last_frame_time;

	void FrameBufferSizeCallBack();
	void MouseCallback(double x_pos, double y_pos);
	void ScrollCallback(double x_offset, double y_offset);
	virtual void ProcessInput();

	friend class WindowInitializer;
};

