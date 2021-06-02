#pragma once
#include "common/WindowFactory.h"

class BasicWindowFactory : public WindowFactory
{
private:
    Window* CreateWindow(GLFWwindow* glfw_window) override;

    //~BasicWindowFactory() override;
};
