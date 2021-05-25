#include "BasicRenderer.h"
#include "common/Shader.h"
#include "common/Transform.h"
#include "common/world.h"

bool BasicRenderer::Initialize()
{
	camera = world->GetCamera();

	return true;
}

void BasicRenderer::BeginRender()
{
	glEnable(GL_DEPTH_TEST);

	basic_shader = new Shader("basic.vs", "basic.fs");

}

void BasicRenderer::Update(float dt)
{
	
}
