#include "WindowManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

template<> WindowManager* Singleton<WindowManager>::singleton = nullptr;
bool WindowManager::Initialize()
{
    if (!glfwInit())
    {
        return false;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    b_initialized = true;
    return true;
}
Window* WindowManager::CreateWindow(int width, int height, const std::string& title)
{
    if (!b_initialized)
    {
        Initialize();
    }

    
    //传进window参数
    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);



    return nullptr;
}
