#include "WindowManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>

#include "imGui/imgui.h"
#include "imGui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"


template<> WindowManager* Singleton<WindowManager>::singleton = nullptr;
Window* WindowManager::s_current_window = nullptr;
vector<Window*> WindowManager::window_array;


WindowManager::WindowManager(int width, int height, const string& title) :
	b_initialized(false)
{
	glfwSetErrorCallback(GLFWErrorCallback);

	if (!glfwInit())
	{
		cout << "ERROR<Window>: glfw init failed.\n";
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	CreateWindow(width, height, title);
	//current_window = 
}

bool WindowManager::Initialize()
{
    b_initialized = true;
    return true;
}

void WindowManager::Update(float dt)
{
	s_current_window->Update(dt);

}
void WindowManager::EndUpdate(float dt)
{
	UpdateImGui();

	s_current_window->EndUpdate();
}
void WindowManager::Terminate()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(s_current_window->glfw_window);
	glfwTerminate();
}
Window* WindowManager::CreateWindow(int width, int height, const std::string& title)
{
	GLFWwindow* glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (glfw_window == nullptr)
	{
		std::cout << "WINDOW ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		
		return nullptr;
	}

	Window* window = new Window(glfw_window);//factory.CreateWindow(glfw_window);
	if (!window)
	{
		glfwTerminate();
		assert(window);
		//return nullptr;
	}
	window_array.push_back(window);
	s_current_window = window;
	//- 生成一个Window的时候自动将上下文切换至新生成的window
	glfwMakeContextCurrent(glfw_window);

	glfwSwapInterval(1); // Enable vsync 启用垂直同步



	glfwSetFramebufferSizeCallback(glfw_window, FrameBufferSizeCallBack);
	glfwSetCursorPosCallback(glfw_window, MouseCallback);
	glfwSetScrollCallback(glfw_window, ScrollCallback);

	glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	if (!b_initialized)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "ERROR<GLAD> from Initialize: Failed to initialize GLAD" << std::endl;
			assert(0);
		}
		b_initialized = true;
	}

	SetupImGui();

	return window;
}

void WindowManager::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); //- 考虑移进window类里
	if (s_current_window)
	{
		if (s_current_window->glfw_window == window)
		{
			s_current_window->FrameBufferSizeCallBack();
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
	if (s_current_window)
	{
		if (s_current_window->glfw_window == window)
		{
			s_current_window->MouseCallback(x_pos, y_pos);
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
	if (s_current_window)
	{
		if (s_current_window->glfw_window == window)
		{
			s_current_window->ScrollCallback(x_offset, y_offset);
		}
		else
		{
			std::cout << "ERROR<Window> from: ScrollCallback\n";
		}
	}
	else
		std::cout << "ERROR<Window> from: ScrollCallback\n";
}

void WindowManager::SetupImGui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(s_current_window->glfw_window, true);
	// GLSL 130
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);


}

void WindowManager::UpdateImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool b_show_demo_window = true;
	ImGui::ShowDemoWindow(&b_show_demo_window);

	bool b_show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &b_show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &b_show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (b_show_another_window)
	{
		ImGui::Begin("Another Window", &b_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			b_show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

