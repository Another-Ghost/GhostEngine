#include "PrefilterShader.h"
#include "RenderManager.h"
#include "WindowManager.h"
#include "CubeMap.h"

PrefilterShader::PrefilterShader()
	:MVPShader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("prefilter_fs"))
{
	
}

void PrefilterShader::RenderPrefilterCubeMap(const CubeMap* prefilter_cubemap, unsigned int ori_cubemap_id)
{
	Use();
	SetInt("environment_map", 0);
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ori_cubemap_id);
	
	shared_ptr<CaptureFrameBuffer> fbo = RenderManager::GetSingleton().GetCaptureFBO();
	//fbo->Bind(prefilter_cubemap->width, prefilter_cubemap->height);
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	
	unsigned int max_mip_levels = 5;
	for (unsigned int mip = 0; mip < max_mip_levels; ++mip)
	{
		// resize framebuffer according to mip-level size.
		unsigned int mip_width = prefilter_cubemap->width * std::pow(0.5f, mip);	//大小很关键
		unsigned int mip_height = prefilter_cubemap->height * std::pow(0.5f, mip);
		//glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mip_width, mip_height);
		fbo->Bind(prefilter_cubemap->width, prefilter_cubemap->height);
		fbo->depth_rbo->Resize(mip_width, mip_height);
		//glViewport(0, 0, mip_width, mip_height);

		float roughness = (float)mip / (float)(max_mip_levels - 1);
		SetFloat("roughness", roughness);

		const vector<mat4>& capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray();
		for (int i = 0; i < 6; ++i)
		{
			SetViewMatrix(capture_view_array[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_cubemap->id, mip); //最后一个参数mip决定渲染到绑定的纹理的哪一层mipmap上

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //? 是否应该被加到每个drawcall之前
			RenderManager::GetSingleton().DrawCaptureCubeMesh(this);
		}

		fbo->Unbind();
	}


	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	//glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);
}

