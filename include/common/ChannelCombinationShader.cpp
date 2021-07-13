#include "ChannelCombinationShader.h"
#include "RenderManager.h"
#include "WindowManager.h"
#include "Texture.h"

ChannelCombinationShader::ChannelCombinationShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	Shader(vertex_path, fragment_path, geometry_path)
{
	SetInt("tex_1", 0);
	SetInt("tex_2", 1);
}

void ChannelCombinationShader::RenderTexture(const Texture* out_tex, const Texture* tex_1, const Texture* tex_2)
{
	Use();
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, out_tex->width, out_tex->height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out_tex->texture_id, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_1->texture_id);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_1->texture_id);

	glViewport(0, 0, out_tex->width, out_tex->height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderManager::GetSingleton().DrawCaptureQuadMesh();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::current_window->GetWidth(), WindowManager::current_window->GetHeight());

	glBindTexture(GL_TEXTURE_2D, out_tex->texture_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

