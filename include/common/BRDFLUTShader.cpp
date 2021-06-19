#include "BRDFLUTShader.h"
#include "Texture.h"

BRDFLUTShader::BRDFLUTShader():
	MVPShader(File::GetShaderPath("brdf_vs"), File::GetShaderPath("brdf_fs"))
{

}

void BRDFLUTShader::RenderBRDFLUT(const Texture* brdf_lut)
{
	Use();

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut->texture_id, 0);



	glViewport(0, 0, brdf_lut->width, brdf_lut->height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderManager::GetSingleton().DrawCaptureQuadMesh();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::current_window->GetWidth(), WindowManager::current_window->GetHeight());
}
