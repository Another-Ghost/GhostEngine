#include "SkyboxShader.h"
#include "Camera.h"
#include "CubeGeometryMesh.h"
#include "RenderManager.h"

SkyboxShader::SkyboxShader():
	MVPShader(File::GetShaderPath("background_vs"), File::GetShaderPath("background_fs"))
{
	Use();
	SetInt("environment_map", 21);
}

void SkyboxShader::RenderSkybox(unsigned int skybox_tex_id)
{
	Use();
	SetViewMatrix(RenderManager::GetSingleton().GetCurrentCameraInfo().view);
	SetProjectionMatrix(RenderManager::GetSingleton().GetCurrentCameraInfo().projection);
	glActiveTexture(GL_TEXTURE21);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_tex_id);
	RenderManager::GetSingleton().DrawCaptureCubeMesh(this);
}
