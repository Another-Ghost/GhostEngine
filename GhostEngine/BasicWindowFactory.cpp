#include "BasicWindowFactory.h"
#include "BasicWindow.h"

Window* BasicWindowFactory::CreateWindow(GLFWwindow* glfw_window)
{
    return new BasicWindow(glfw_window);
}
