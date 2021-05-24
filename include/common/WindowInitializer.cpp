#include "WindowInitializer.h"
#include "Window.h"
#include <stb_image.h>
#include <iostream>



WindowInitializer::WindowInitializer()
{
	Init();
}

bool WindowInitializer::Init()
{
	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

}

Window* WindowInitializer::CreateWindow(Window* win, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//? 判断一下glfwwindow是不是在堆里生成的
	//estimate weather glfwwindow is created in heap or not
	GLFWwindow* glfw_window = glfwCreateWindow(width, height, "Demo", NULL, NULL); 

	if (glfw_window == nullptr)
	{
		std::cout << "WINDOW ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	window = new Window(glfw_window, width, height);	
	glfwMakeContextCurrent(glfw_window);

	glfwSetFramebufferSizeCallback(glfw_window, FrameBufferSizeCallBack);
	glfwSetCursorPosCallback(glfw_window, MouseCallback);
	glfwSetScrollCallback(glfw_window, ScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




    return window;
}


void WindowInitializer::FrameBufferSizeCallBack(GLFWwindow* glfw_window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void WindowInitializer::MouseCallback(GLFWwindow* glfw_window, double x_pos, double y_pos)
{
	window->MouseCallback(x_pos, y_pos);
}

void WindowInitializer::ScrollCallback(GLFWwindow* glfw_window, double x_offset, double y_offset)
{
	window->ScrollCallback(x_offset, y_offset);
}

