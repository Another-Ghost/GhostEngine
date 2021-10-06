#include "ReflectionProbeShader.h"
#include "RenderManager.h"
#include "CubeMap.h"

ReflectionProbeShader::ReflectionProbeShader(const string& vertex_path /*= File::GetShaderPath("cubemap_vs")*/, const string& fragment_path /*= File::GetShaderPath("reflection_probe_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{

}


void ReflectionProbeShader::Draw(CubeMap* cubemap)
{

	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	//glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemap->width, cubemap->height);
	shared_ptr<CaptureFrameBuffer> fbo = RenderManager::GetSingleton().GetCaptureFBO();
	fbo->Bind(cubemap->width, cubemap->height);
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjectionMatrix());

	const vector<mat4>& capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray();
	for (unsigned int i = 0; i < 6; ++i)
	{
		SetViewMatrix(capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager::GetSingleton().DrawCaptureCubeMesh(this);
	}
	
	fbo->Unbind();
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	//glViewport(0, 0, WindowManager::s_current_window->GetWidth(), WindowManager::s_current_window->GetHeight());
	//glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);
}
