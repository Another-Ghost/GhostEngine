#pragma once
#include "common/WindowFactory.h"

class BasicWindowFactory :
    public WindowFactory
{
    Window* CreateWindow(GLFWwindow* glfw_window) override;
};

