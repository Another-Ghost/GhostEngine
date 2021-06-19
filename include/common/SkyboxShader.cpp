#include "SkyboxShader.h"
#include "Camera.h"
#include "CubeGeometryMesh.h"

SkyboxShader::SkyboxShader():
	MVPShader(File::GetShaderPath("background_vs"), File::GetShaderPath("background_fs"))
{
	Use();
	SetInt("environment_map", 0);
}

void SkyboxShader::RenderSkybox(unsigned int skybox_tex_id)
{
	Use();
	SetViewMatrix(RenderManager::GetSingleton().camera->ViewMatrix());
	SetProjectionMatrix(RenderManager::GetSingleton().camera->PerspectiveMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_id);
	RenderManager::GetSingleton().DrawCaptureCubeMesh();
}
