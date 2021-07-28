#include "GBuffer.h"
#include "AttachmentTexture.h"
#include "ResourceManager.h"
#include "WindowManager.h"

GBuffer::GBuffer()
{
	glGenFramebuffers(1, &fbo);
	glObjectLabel(GL_FRAMEBUFFER, fbo, -1, "gbuffer_fbo");
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	world_position_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, true));	//因为需要采样数据，所以position等使用纹理附件而不是渲染缓冲对象附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, world_position_tex->id, 0);	//附加纹理到FBO中
	glObjectLabel(GL_TEXTURE, world_position_tex->id, -1, "world_position_tex");

	world_normal_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
	//normal_g_attachment->wrap_param = GL_REPEAT;
	world_normal_tex->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, world_normal_tex->id, 0);
	glObjectLabel(GL_TEXTURE, world_normal_tex->id, -1, "world_normal_tex");

	basecolor_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
	basecolor_tex->internal_format = GL_RGBA;
	basecolor_tex->data_type = GL_UNSIGNED_BYTE;
	//normal_g_attachment->wrap_param = GL_REPEAT;
	basecolor_tex->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, basecolor_tex->id, 0);
	glObjectLabel(GL_TEXTURE, basecolor_tex->id, -1, "basecolor_tex");

	view_position_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, true));	//因为需要采样数据，所以position等使用纹理附件而不是渲染缓冲对象附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, view_position_tex->id, 0);	//附加纹理到FBO中
	glObjectLabel(GL_TEXTURE, view_position_tex->id, -1, "view_position_tex");

	view_normal_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, true));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, view_normal_tex->id, 0);
	glObjectLabel(GL_TEXTURE, view_normal_tex->id, -1, "view_normal_tex");

	ao_roughness_metalness_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
	//ao_roughness_metalness_tex->internal_format = GL_RGB16F;
	ao_roughness_metalness_tex->data_type = GL_UNSIGNED_BYTE;
	ao_roughness_metalness_tex->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, ao_roughness_metalness_tex->id, 0);
	glObjectLabel(GL_TEXTURE, ao_roughness_metalness_tex->id, -1, "ao_roughness_metalness_tex");

	emissive_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
	//emissive_tex->internal_format = GL_RGB16F;
	emissive_tex->data_type = GL_UNSIGNED_BYTE;
	emissive_tex->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, emissive_tex->id, 0);
	glObjectLabel(GL_TEXTURE, emissive_tex->id, -1, "emissive_tex");


	color_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
	color_tex->internal_format = GL_RGBA;
	color_tex->data_type = GL_UNSIGNED_BYTE;
	color_tex->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, color_tex->id, 0);
	glObjectLabel(GL_TEXTURE, color_tex->id, -1, "color_tex");

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLenum attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	glDrawBuffers(8, attachments);

	glGenRenderbuffers(1, &depth_rbo);
	glObjectLabel(GL_RENDERBUFFER, depth_rbo, -1, "depth_rbo");
	glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowManager::s_current_window->GetWidth(), WindowManager::s_current_window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR<GBuffer>: G-Buffer not complete!" << std::endl;
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}	//提取出FrameBuffer基类，移动到基类里

void GBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
