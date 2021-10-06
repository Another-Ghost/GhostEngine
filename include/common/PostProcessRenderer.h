#pragma once
#include "Renderer.h"
class PostProcessRenderer : public Renderer
{
public:
	PostProcessRenderer(int width, int height);	//? 把framebuffer分离出去

	void Update(float dt) override;
	
	void UpdateSSAO();

private:
	/*SSAO*/
	GLuint ssao_kernel_ssbo;

	unique_ptr<FrameBuffer> ssao_fbo;
	unique_ptr<FrameBuffer> ssao_blur_fbo;
	PlaneTexture* ssao_color_attachment;
	PlaneTexture* ssao_blur_color_attachment;

	vector<vec4> ssao_kernel;

	int sample_num;
	PlaneTexture* noise_texture;
	int noise_tex_size{ 4 };

	SSAOShader* ssao_shader;
	SSAOBlurShader* ssao_blur_shader;

	OutputShader* output_shader;
};

