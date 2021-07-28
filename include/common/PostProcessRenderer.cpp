#include "PostProcessRenderer.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Math.h"
#include "SSAOShader.h"
#include "SSAOBlurShader.h"
#include "OutputShader.h"
#include "AttachmentTexture.h"
#include "GBuffer.h"
#include <random>

PostProcessRenderer::PostProcessRenderer()
{
	glGenBuffers(1, &ssao_kernel_ssbo);
	glObjectLabel(GL_BUFFER, ssao_kernel_ssbo, -1, "ssao_kernel_ssbo");
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssao_kernel_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssao_kernel_ssbo);	// binding point is 2
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	/*SSAO*/
	glGenFramebuffers(1, &ssao_fbo);
	glObjectLabel(GL_FRAMEBUFFER, ssao_fbo, -1, "ssao_fbo");
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
	ssao_color_attachment = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false);
	ssao_color_attachment->internal_format = GL_RED;
	ssao_color_attachment->data_format = GL_RED;
	ssao_color_attachment->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_color_attachment->id, 0);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Frame buffer not complete!" << std::endl;
#endif

	glGenFramebuffers(1, &ssao_blur_fbo);
	glObjectLabel(GL_FRAMEBUFFER, ssao_blur_fbo, -1, "ssao_blur_fbo");
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_fbo);
	ssao_blur_color_attachment = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false);
	ssao_blur_color_attachment->internal_format = GL_RED;
	ssao_blur_color_attachment->data_format = GL_RED;
	ssao_blur_color_attachment->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_blur_color_attachment->id, 0);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Frame buffer not complete!" << std::endl;
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::uniform_real_distribution<GLfloat> random_floats(0.f, 1.f);	//均匀分布
	std::default_random_engine generator;	//默认随机数生成器（由编译器定，大概率是线性同余（RandSeed = (A * RandSeed + B) % M，伪随机））
	for (int i = 0; i < 64; ++i)
	{
		vec3 sample{ random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator) };	//随机方向（切线空间）
		sample = glm::normalize(sample);
		sample *= random_floats(generator);	//随机半径大小
		float scale = float(i) / 64;

		scale = Math::Lerp<float>(0.1f, 1.0f, scale * scale);	//加速插值函数，使核的样本靠近原点分布
		sample *= scale;
		ssao_kernel.emplace_back(sample, 1.f);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssao_kernel_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * ssao_kernel.size(), &ssao_kernel[0], GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// generate noise texture
	vector<vec3> ssao_noise;
	//constexpr int noize_size = noise_tex_size * noise_tex_size * 3;
	//float ssao_rotate_noise[noize_size];

	//vector<float> ssao_rotate_noise;
	//ssao_rotate_noise.resize(noise_tex_size * noise_tex_size * 3);
	for (unsigned int i = 0; i < noise_tex_size * noise_tex_size; ++i)
	{
		//glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f);	//在x-y平面上，绕z轴均匀分布
		ssao_noise.emplace_back(vec3(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f));
		//ssao_rotate_noise[i * 3] = random_floats(generator) * 2.0 - 1.0;
		//ssao_rotate_noise[i * 3 + 1] = random_floats(generator) * 2.0 - 1.0;
		//ssao_rotate_noise[i * 3 + 2] = 0.0f;
	}

	HDRTextureFile* noise_tex_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::HDR, false));
	noise_tex_file->width = noise_tex_size;
	noise_tex_file->height = noise_tex_size;
	noise_tex_file->format = GL_RGB;
	//noise_tex_file->component_num = 3;
	noise_tex_file->data = &ssao_noise[0].x; //? 可能存在问题
	//noise_tex_file->data = &ssao_rotate_noise[0];

	noise_tex_file->b_loaded = true;

	noise_texture = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::EMPTY2D, false, noise_tex_file);
	glObjectLabel(GL_TEXTURE, noise_texture->id, -1, "noise_texture");
	noise_texture->internal_format = GL_RGBA32F;
	noise_texture->min_filter_param = GL_NEAREST;
	noise_texture->mag_filter_param = GL_NEAREST;
	noise_texture->wrap_param = GL_REPEAT;
	noise_texture->Buffer();

	ssao_shader = new SSAOShader();
	ssao_shader->SetInt("noise_tex_size", noise_tex_size); //? 改成在自己的draw函数里调用
	ssao_blur_shader = new SSAOBlurShader();

	output_shader = new OutputShader();
}

void PostProcessRenderer::Update(float dt)
{
	UpdateSSAO();
}

void PostProcessRenderer::UpdateSSAO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	TextureUnit::Bind2DTexture(TextureUnit::g_view_position, RenderManager::GetSingleton().g_buffer->view_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_view_normal, RenderManager::GetSingleton().g_buffer->view_normal_tex);
	TextureUnit::Bind2DTexture(SSAOShader::noise_texture_tu, noise_texture);
	RenderManager::GetSingleton().DrawCaptureQuadMesh(ssao_shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_fbo);
	glClear(GL_COLOR_BUFFER_BIT);
	TextureUnit::Bind2DTexture(TextureUnit::ssao, ssao_color_attachment);
	RenderManager::GetSingleton().DrawCaptureQuadMesh(ssao_blur_shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TextureUnit::Bind2DTexture(TextureUnit::g_color, RenderManager::GetSingleton().color_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_color, RenderManager::GetSingleton().g_buffer->color_tex);

	TextureUnit::Bind2DTexture(TextureUnit::ssao, ssao_blur_color_attachment);
	RenderManager::GetSingleton().DrawCaptureQuadMesh(output_shader);
}
