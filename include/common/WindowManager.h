#pragma once
#include "Singleton.h"
#include "Window.h"
#include <vector>
#include <string>

class WindowManager : public Singleton<WindowManager>
{
public:
	bool Initialize();

	Window* CreateWindow(int width, int height, const std::string& title);

	static std::vector<Window*> window_vector;

	bool b_initialized;
};

