#include "RenderManager.h"
#include "RenderUnit.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "BasicMaterial.h"
#include "PBRMaterial.h"
#include "Camera.h"
#include "Mesh.h"
#include "IBLRenderer.h"
#include "BasicRenderer.h"
#include "CubeGeometryMeshFactory.h"
#include "ResourceManager.h"
#include "CubeMap.h"
#include "TextureFile.h"
#include "EquirectangularMap.h"
#include "HDRIShader.h"
#include "File.h"
#include "ChannelCombinationShader.h"
#include "Math.h"
#include "SSAOShader.h"
#include "SSAOBlurShader.h"
#include <random>

template<> RenderManager* Singleton<RenderManager>::singleton = nullptr;
RenderManager::RenderManager():
	b_skybox_initialized(false)
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenFramebuffers(1, &capture_fbo);	
	glGenRenderbuffers(1, &capture_rbo);

	int win_width = WindowManager::s_current_window->GetWidth();
	int win_height = WindowManager::s_current_window->GetHeight();

/*global variables*/
	capture_view_array =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	capture_projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
	capture_cube_mesh = dynamic_cast<CubeGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(CubeGeometryMeshFactory()));

	capture_quad_mesh = new QuadGeometryMesh();

	LDRTextureFile* brdf_lut_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("system/brdf_lut.png")));
	brdf_lut = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D, true, brdf_lut_file);
	//brdf_lut->data_format = GL_RG;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	glGenBuffers(1, &camera_ubo);	// binding point is 0	//? 写成一个类
	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	WindowInfo win_info{ win_width, win_height };
	glGenBuffers(1, &window_ubo);	// binding point is 0	//? 写成一个类
	glBindBuffer(GL_UNIFORM_BUFFER, window_ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, window_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(WindowInfo), &win_info, GL_STATIC_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	glGenBuffers(1, &light_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_ssbo);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ssbo);	// binding point is 1
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	glGenBuffers(1, &ssao_kernel_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssao_kernel_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssao_kernel_ssbo);	// binding point is 2
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

/*G-Buffer*/
	
	glGenFramebuffers(1, &gbuffer_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	
	position_g_attachment = ResourceManager::GetSingleton().CreateTexture(TextureType::ATTACHMENT, true);	//因为需要采样数据，所以position等使用纹理附件而不是渲染缓冲对象附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_g_attachment->id, 0);	//附加纹理到FBO中

	normal_g_attachment = ResourceManager::GetSingleton().CreateTexture(TextureType::ATTACHMENT, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_g_attachment->id, 0);

	normal_g_attachment = ResourceManager::GetSingleton().CreateTexture(TextureType::ATTACHMENT, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normal_g_attachment->id, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glGenRenderbuffers(1, &gbuffer_depth_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, gbuffer_depth_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, win_width, win_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbuffer_depth_rbo);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR<RenderManager>: Frame buffer not complete!" << std::endl;
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

/*SSAO*/
	glGenFramebuffers(1, &ssao_fbo);  
	glGenFramebuffers(1, &ssao_blur_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);

	ssao_color_attachment = ResourceManager::GetSingleton().CreateTexture(TextureType::ATTACHMENT, false);
	ssao_color_attachment->internal_format = GL_RED;
	ssao_color_attachment->data_format = GL_RED;
	ssao_color_attachment->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_color_attachment->id, 0);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Frame buffer not complete!" << std::endl;
#endif

	ssao_blur_color_attachment = ResourceManager::GetSingleton().CreateTexture(TextureType::ATTACHMENT, false);
	ssao_blur_color_attachment->internal_format = GL_RED;
	ssao_blur_color_attachment->data_format = GL_RED;
	ssao_blur_color_attachment->Buffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_blur_color_attachment->id, 0);
#ifdef DEBUG_MODE
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Frame buffer not complete!" << std::endl;
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::uniform_real_distribution<GLfloat> random_floats;	//均匀分布
	std::default_random_engine generator;	//默认随机数生成器（由编译器定，大概率是线性同余（RandSeed = (A * RandSeed + B) % M，伪随机））
	for (int i = 0; i < 64; ++i)
	{
		vec3 sample{ random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator) };	//随机方向（切线空间）
		sample = glm::normalize(sample);
		sample *= random_floats(generator);	//随机半径大小
		float scale = float(i) / 64;

		scale = Math::Lerp<float>(0.1f, 1.0f, scale * scale);	//加速插值函数，使核的样本靠近原点分布
		sample *= scale;
		ssao_kernel.emplace_back(sample, 0.f);
	}

	// generate noise texture
	std::vector<vec3> ssao_noise;
	for (unsigned int i = 0; i < 16; ++i)
	{
		glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f);	//在x-y平面上，绕z轴均匀分布
		ssao_noise.push_back(noise);
	}
	
	HDRTextureFile* noise_tex_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::HDR, false));
	noise_tex_file->width = 4;
	noise_tex_file->height = 4;
	noise_tex_file->format = GL_FLOAT;
	//noise_tex_file->component_num = 3;
	noise_tex_file->data = &ssao_noise[0].x; //? 可能存在问题
	noise_tex_file->b_loaded = true;

	noise_texture = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D, false, noise_tex_file);
	noise_texture->internal_format = GL_RGBA32F;
	noise_texture->min_filter_param = GL_NEAREST;
	noise_texture->mag_filter_param = GL_NEAREST;
	noise_texture->wrap_param = GL_REPEAT;
	noise_texture->Buffer();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssao_kernel_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * ssao_kernel.size(), &ssao_kernel[0], GL_STATIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	ssao_shader = new SSAOShader();
	ssao_blur_shader = new SSAOBlurShader();

	channel_combination_shader = new ChannelCombinationShader();

}

bool RenderManager::Initialize(Renderer* renderer_)
{
	if (!b_initialized)
	{
		if (renderer_ == nullptr)
		{
			renderer_ = new BasicRenderer();
		}
		current_renderer = renderer_;

		b_initialized = true;

		basic_renderer = new BasicRenderer();
	}

	return b_initialized;
}



void RenderManager::Update(float dt)
{
	UpdateLightArray();

	UpdateCamera();

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
	current_renderer->Update(dt);
	
	basic_renderer->Update(dt);
	//glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);


}

void RenderManager::UpdateCamera()
{

	camera = SceneManager::GetSingleton().main_camera;

	CameraInfo camera_info;
	camera_info.position = { camera->GetPosition(), 0 };
	camera_info.view = camera->ViewMatrix();
	camera_info.projection = camera->PerspectiveMatrix();

	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &camera_info, GL_STREAM_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void RenderManager::UpdateLightArray()
{

	vector<PointLightInfo> light_info_array;
	for (const auto& light : SceneManager::GetSingleton().light_array)
	{
		if (dynamic_cast<PointLight*>(light) && light->b_enabled)
		{
			PointLightInfo light_info;
			light_info.position = { light->postion, 0.f };
			light_info.color = { light->color * light->intensity, 0.f };
			light_info_array.emplace_back(light_info);
		}
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLightInfo)* light_info_array.size(), &light_info_array[0], GL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

void RenderManager::UpdateSSAO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
	TextureUnit::Bind2DTexture(TextureUnit::g_position, position_g_attachment);
	TextureUnit::Bind2DTexture(TextureUnit::g_normal, noise_texture);
	TextureUnit::Bind2DTexture(SSAOShader::noise_texture_tu, noise_texture);
	capture_quad_mesh->Draw(ssao_shader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}


void RenderManager::ResetRenderArray()
{
	pbr_mat_unit_map.clear();
}

void RenderManager::InsertRenderUnit(RenderUnit* ru)
{
	Material* material = ru->GetMaterial();
	if (material->type == MaterialType::BASIC)
	{
		BasicMaterial* basic_mat = dynamic_cast<BasicMaterial*>(material);
		if (basic_mat_unit_map.count(basic_mat))
		{
			basic_mat_unit_map[basic_mat].emplace(ru);
		}
		else
		{
			basic_mat_unit_map.emplace(basic_mat, set<RenderUnit*>());
		}
	}
	else if(material->type == MaterialType::PBR)
	{
		PBRMaterial* pbr_mat = dynamic_cast<PBRMaterial*>(material);
		auto ru_set_ite = pbr_mat_unit_map.find(pbr_mat);
		if (ru_set_ite == pbr_mat_unit_map.end())
		{
			pbr_mat_unit_map.emplace(pbr_mat, set<RenderUnit*>());
		}
		pbr_mat_unit_map[pbr_mat].emplace(ru);
	}

}

void RenderManager::BindSkyboxTexture(HDRTextureFile* hdr_file)
{
	if (!b_skybox_initialized)
	{
		skybox_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));
		skybox_cubemap->width = 1024;
		skybox_cubemap->height = 1024;
		skybox_cubemap->b_genarate_mipmap = false;
		skybox_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
		skybox_cubemap->Buffer();
	}

	//HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/old_hall.hdr"), TextureFileType::HDR));
	EquirectangularMap* equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::EQUIRECTANGULARMAP, true, hdr_file));	//? 先删除旧的
	//equirectanguler_map->Buffer();

	HDRIShader hdri_cubemap_shader;
	hdri_cubemap_shader.RenderCubeMap(skybox_cubemap, equirectanguler_map->id);

}

void RenderManager::CombineChannels(Texture* out_tex, Texture* tex1, Texture* tex2)
{
	channel_combination_shader->RenderTexture(out_tex, tex1, tex2);
}


