#include "ShadowRenderer.h"
#include "PointShadowDepthShader.h"
//#include "PointShadowShader.h"
#include "CubeMap.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "PointLight.h"


ShadowRenderer::ShadowRenderer()
{
	point_shadow_depth_shader = new PointShadowDepthShader();
	//point_shadow_shader = make_unique<PointShadowShader>();

	glGenFramebuffers(1, &depth_map_fbo);
	
	//depth_cubemaps = ResourceManager::GetSingleton().CreateCubemap(shadow_width, shadow_height);
	//depth_cubemaps->internal_format = GL_DEPTH_COMPONENT;
	//depth_cubemaps->data_format = GL_DEPTH_COMPONENT;
	//depth_cubemaps->Buffer();

	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_cubemaps->id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());

	//float near_plane = 1.f;
	//float far_plane = 25.f;
	shadow_projection = glm::perspective(glm::radians(90.0f), (float)shadow_width / (float)shadow_height, near_plane, far_plane);
	
}

void ShadowRenderer::Update(float dt)
{
}

void ShadowRenderer::DrawDepthMap(PointLight* light)
{

	vector<mat4> view_matrices = RenderManager::GetSingletonPtr()->GetCaptureViewArray(light->GetPosition());
	
	for (int i = 0; i < 6; ++i)
	{
		shadow_matrices.emplace_back(shadow_projection * view_matrices[i]);
	}

	glViewport(0, 0, shadow_width, shadow_height);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, light->shadow_cubemap->id, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);

	point_shadow_depth_shader->SetShadowMatrix(shadow_matrices);
	point_shadow_depth_shader->SetFloat("far_plane", far_plane);
	point_shadow_depth_shader->SetVec3("light_pos", light->GetPosition());
	//RenderManager::GetSingleton().DrawCaptureCubeMesh(&*point_shadow_depth_shader);
	RenderManager::GetSingletonPtr()->DrawMeshes(&*point_shadow_depth_shader);

	glTextureBarrier();

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);

	
}

