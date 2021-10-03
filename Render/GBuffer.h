#pragma once
#include "Prerequisite.h"

struct GBuffer
{
	unsigned int fbo;
	unsigned int depth_rbo;

	AttachmentTexture* world_position_tex;
	AttachmentTexture* world_normal_tex;
	AttachmentTexture* basecolor_tex;
	AttachmentTexture* view_position_tex;
	AttachmentTexture* view_normal_tex;
	AttachmentTexture* ao_roughness_metalness_tex;
	AttachmentTexture* emissive_tex;
	AttachmentTexture* color_tex;


	GBuffer();

	GBuffer(int width, int height);

	void Bind();


	void Unbind();
	
};

