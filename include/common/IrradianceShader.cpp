#include "IrradianceShader.h"
#include "CubeMap.h"
#include "RenderManager.h"

IrradianceShader::IrradianceShader():
	MVPShader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("irradiance_convolution_fs"))
{

}

void IrradianceShader::RenderEnvIrradianceCubeMap(const CubeMap* irradiance_cubemap, unsigned int ori_cubemap_id)
{
	Use();

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradiance_cubemap->width, irradiance_cubemap->height);

	SetInt("environment_map", 0);
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ori_cubemap_id);

	glViewport(0, 0, irradiance_cubemap->width, irradiance_cubemap->height);
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	const vector<mat4>& capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray();
	for (unsigned int i = 0; i < 6; ++i)
	{
		SetViewMatrix(capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_cubemap->id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager::GetSingleton().DrawCaptureCubeMesh(this);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	glViewport(0, 0, RenderManager::GetSingleton().GetViewportInfo().width, RenderManager::GetSingleton().GetViewportInfo().height);
}


