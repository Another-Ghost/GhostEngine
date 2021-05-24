#include "BasicRenderer.h"
#include "common/Shader.h"


BasicRenderer::BasicRenderer()
{
	window = WindowInitializer().CreateWindow(1280, 720);
}

bool BasicRenderer::Init()
{
	glEnable(GL_DEPTH_TEST);

	basic_shader = new Shader("basic.vs", "basic.fs");
	return true;
}

bool BasicRenderer::Update(float& dt)
{
	
}
