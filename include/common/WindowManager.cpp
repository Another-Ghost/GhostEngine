#include "WindowManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

template<> WindowManager* Singleton<WindowManager>::singleton = nullptr;
Window* WindowManager::current_window = nullptr;


bool WindowManager::Initialize()
{
    if (!glfwInit())
    {
        return false;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    b_initialized = true;
    return true;
}
void WindowManager::Update(float dt)
{
	current_window->Update(dt);

}
void WindowManager::EndUpdate(float dt)
{
	current_window->EndUpdate();
}
void WindowManager::Terminate()
{
	glfwTerminate();
}
Window* WindowManager::CreateWindow(WindowFactory* factory, int width, int height, const std::string& title)
{
	if (!b_initialized)
	{
		if (!Initialize())
			return nullptr;
	}

	GLFWwindow* glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (glfw_window == nullptr)
	{
		std::cout << "WINDOW ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		
		return nullptr;
	}

	Window* window = factory->CreateWindow(glfw_window);
	if (!window)
	{
		glfwTerminate();
		assert(window);
		//return nullptr;
	}
	window_array.push_back(window);
	current_window = window;
	//- 生成一个Window的时候自动将上下文切换至新生成的window
	glfwMakeContextCurrent(glfw_window);

	glfwSetFramebufferSizeCallback(glfw_window, FrameBufferSizeCallBack);
	glfwSetCursorPosCallback(glfw_window, MouseCallback);
	glfwSetScrollCallback(glfw_window, ScrollCallback);

	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR<GLAD> from Initialize: Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	return window;
}

void WindowManager::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //- 考虑移进window类里
	if (current_window)
	{
		if (current_window->glfw_window == window)
		{
			current_window->FrameBufferSizeCallBack();
		}
		else
		{
			std::cout << "ERROR<Window> from: FrameBufferSizeCallBack\n";
		}
	}
	else
		std::cout << "ERROR<Window> from: FrameBufferSizeCallBack\n";
}

void WindowManager::MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (current_window)
	{
		if (current_window->glfw_window == window)
		{
			current_window->MouseCallback(x_pos, y_pos);
		}
		else
		{
			std::cout << "ERROR<Window> from: MouseCallback\n";
		}
	}
	else
		std::cout << "ERROR<Window> from: MouseCallback\n";
}

void WindowManager::ScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	if (current_window)
	{
		if (current_window->glfw_window == window)
		{
			current_window->ScrollCallback(x_offset, y_offset);
		}
		else
		{
			std::cout << "ERROR<Window> from: ScrollCallback\n";
		}
	}
	else
		std::cout << "ERROR<Window> from: ScrollCallback\n";
}

