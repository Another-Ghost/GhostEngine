#include "BasicRenderer.h"
#include "RenderManager.h"
#include "MVPShader.h"
#include "Camera.h"

BasicRenderer::BasicRenderer()
{
	//basic_shader = new MVPShader(File::GetShaderPath("basic_v"), File::GetShaderPath("basic_f"));
}

void BasicRenderer::Update(float dt)
{
	Camera* camera = RenderManager::GetSingleton().camera;
	basic_shader->SetProjectionMatrix(camera->PerspectiveMatrix());
	basic_shader->SetViewMatrix(camera->ViewMatrix());
	//basic_shader->SetCameraPosition(camera->GetPosition());
}
