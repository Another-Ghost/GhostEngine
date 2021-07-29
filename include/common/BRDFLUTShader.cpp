#include "BRDFLUTShader.h"
#include "PlaneTexture.h"
#include "RenderManager.h"

BRDFLUTShader::BRDFLUTShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
}

void BRDFLUTShader::RenderBRDFLUT(const PlaneTexture* brdf_lut)
{
	Use();

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut->id, 0);

	glViewport(0, 0, brdf_lut->width, brdf_lut->height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderManager::GetSingleton().DrawCaptureQuadMesh(this);
	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);
}
