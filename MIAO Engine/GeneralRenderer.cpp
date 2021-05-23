#include "GeneralRenderer.h"
#include "common/Shader.h"


bool GeneralRenderer::Init()
{
	glEnable(GL_DEPTH_TEST);

	basic_shader = Shader("basic.vs", "basic.fs");
	return true;
}

void GeneralRenderer::Update()
{

}
