#include "LightProbeRenderer.h"
#include "GBuffer.h"
#include "PostProcessRenderer.h"
#include "ReflectionProbe.h"
#include "RenderManager.h"
#include "CubeMap.h"
#include "PBRDeferRenderer.h"

LightProbeRenderer::LightProbeRenderer(int width, int height)
	:viewport_width(width), viewport_height(height)
{
	g_buffer = make_shared<GBuffer>(viewport_width, viewport_height);
	post_process_renderer = make_unique<PostProcessRenderer>(viewport_width, viewport_height);

	glGenFramebuffers(1, &capture_fbo);
	glGenRenderbuffers(1, &capture_rbo);
}

void LightProbeRenderer::Render(ReflectionProbe* probe)
{
	//g_buffer->Bind();
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, probe->depth_cubemap->id, 0);
	//g_buffer->Unbind();

	ViewportInfo viewport_info{ viewport_width, viewport_height };
	RenderManager::GetSingleton().ModifyCurrentViewportInfo(viewport_info);
	RenderManager::GetSingleton().cur_g_buffer = g_buffer;

	//RenderManager::GetSingleton().SetCurrentOutputFrameBuffer(capture_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, viewport_width, viewport_height);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, probe->depth_cubemap->id, 0);

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());

	//glViewport(0, 0, viewport_width, viewport_height);
	const vector<mat4> capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray(probe->GetWorldTransform().GetPosition());

	//camera_info.projection = RenderManager::GetSingleton().probe_capture_projection;
	//camera_info.projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, probe->aabb_module->volume.half_size.x);	//? 改为可适应任意形状
	camera_info.projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 100.f);	//? 改为可适应任意形状

	//vec3 probe_pos = probe->GetWorldTransform().GetPosition();
	camera_info.position = { probe->GetWorldTransform().GetPosition(), 1 };

	//glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);


	for (unsigned int i = 0; i < 6; ++i)
	{
		g_buffer->Bind();
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->depth_cubemap->id, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->world_pos_cubemap->id, 0);	//?
		g_buffer->Unbind();

		glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);

		camera_info.view = capture_view_array[i];
		RenderManager::GetSingleton().ModifyCurrentCameraInfo(camera_info);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->depth_cubemap->id, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->cubemap->id, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//RenderManager::GetSingleton().pbr_defer_renderer->RenderDirectLight(); 
		post_process_renderer->Update(0);
		//glTextureBarrier();
	}

	

	RenderManager::GetSingleton().UpdateCamera();
	//RenderManager::GetSingleton().SetCurrentOutputFrameBuffer(0);
	RenderManager::GetSingleton().cur_g_buffer = RenderManager::GetSingleton().g_buffer;
	RenderManager::GetSingleton().ModifyCurrentViewportInfo(RenderManager::GetSingleton().GetWindowViewportInfo());

	probe->Initialize();
}
