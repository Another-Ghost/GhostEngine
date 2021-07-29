#include "LightProbeRenderer.h"
#include "GBuffer.h"
#include "PostProcessRenderer.h"
#include "LightProbe.h"
#include "RenderManager.h"
#include "CubeMap.h"

LightProbeRenderer::LightProbeRenderer(int width, int height)
	:viewport_width(width), viewport_height(height)
{
	g_buffer = make_shared<GBuffer>(viewport_width, viewport_height);
	post_process_renderer = make_unique<PostProcessRenderer>(viewport_width, viewport_height);

}

void LightProbeRenderer::Render(LightProbe* probe)
{

	ViewportInfo viewport_info{ viewport_width, viewport_height };
	RenderManager::GetSingleton().ModifyCurrentViewportInfo(viewport_info);
	RenderManager::GetSingleton().cur_g_buffer = g_buffer;

	RenderManager::GetSingleton().SetCurrentOutputFrameBuffer(RenderManager::GetSingleton().GetCaptureFBO());
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, viewport_width, viewport_height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());

	//glViewport(0, 0, viewport_width, viewport_height);
	const vector<mat4> capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray(probe->GetWorldTransform().GetPosition());

	camera_info.projection = RenderManager::GetSingleton().probe_capture_projection;
	//vec3 probe_pos = probe->GetWorldTransform().GetPosition();
	camera_info.position = { probe->GetWorldTransform().GetPosition(), 1 };

	for (unsigned int i = 0; i < 6; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());

		camera_info.view = capture_view_array[i];
		RenderManager::GetSingleton().ModifyCurrentCameraInfo(camera_info);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->cubemap->id, 0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager::GetSingleton().Render(0);
		post_process_renderer->Update(0);
		//glTextureBarrier();
	}


	RenderManager::GetSingleton().UpdateCamera();
	RenderManager::GetSingleton().SetCurrentOutputFrameBuffer(0);
	RenderManager::GetSingleton().cur_g_buffer = RenderManager::GetSingleton().g_buffer;
	RenderManager::GetSingleton().ModifyCurrentViewportInfo(RenderManager::GetSingleton().GetWindowViewportInfo());
}
