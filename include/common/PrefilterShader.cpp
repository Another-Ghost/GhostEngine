#include "PrefilterShader.h"
#include "RenderManager.h"
#include "CubeMap.h"

PrefilterShader::PrefilterShader()
	:MVPShader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("prefilter_fs"))
{
	
}

void PrefilterShader::RenderPrefilterCubeMap(const CubeMap* prefilter_cubemap, unsigned int ori_cubemap_id)
{
	Use();
	SetInt("environment_map", 0);
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjecctionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ori_cubemap_id);

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	unsigned int max_mip_levels = 5;
	for (unsigned int mip = 0; mip < max_mip_levels; ++mip)
	{
		// resize framebuffer according to mip-level size.
		unsigned int mip_width = prefilter_cubemap->width * std::pow(0.5f, mip);	//大小很关键
		unsigned int mip_height = prefilter_cubemap->height * std::pow(0.5f, mip);
		//glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mip_width, mip_height);
		glViewport(0, 0, mip_width, mip_height);

		float roughness = (float)mip / (float)(max_mip_levels - 1);
		SetFloat("roughness", roughness);

		const vector<mat4>& capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray();
		for (int i = 0; i < 6; ++i)
		{
			SetViewMatrix(capture_view_array[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_cubemap->texture_id, mip); //最后一个参数mip决定渲染到绑定的纹理的哪一层mipmap上

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //? 是否应该被加到每个drawcall之前
			RenderManager::GetSingleton().DrawCaptureCubeMesh();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::current_window->GetWidth(), WindowManager::current_window->GetHeight());
}

