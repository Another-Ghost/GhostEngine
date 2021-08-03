#include "ChannelCombinationShader.h"
#include "RenderManager.h"
#include "PlaneTexture.h"

ChannelCombinationShader::ChannelCombinationShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	Shader(vertex_path, fragment_path, geometry_path)
{
	Use();
	SetInt("tex1", 23);
	SetInt("tex2", 24);
}

void ChannelCombinationShader::RenderTexture(const PlaneTexture* out_tex, const PlaneTexture* tex1, const PlaneTexture* tex2)
{
	Use();
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, out_tex->width, out_tex->height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out_tex->id, 0);

	glActiveTexture(GL_TEXTURE23);
	glBindTexture(GL_TEXTURE_2D, tex1->id);

	glActiveTexture(GL_TEXTURE24);
	glBindTexture(GL_TEXTURE_2D, tex2->id);

	glViewport(0, 0, out_tex->width, out_tex->height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderManager::GetSingleton().DrawCaptureQuadMesh(this);
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());	
	glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);	//? buffer 类统一管理

	glBindTexture(GL_TEXTURE_2D, out_tex->id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

