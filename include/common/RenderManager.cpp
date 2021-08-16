#include "RenderManager.h"
#include "RenderUnit.h"
#include "WindowManager.h"
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
#include "SkyboxShader.h" 
#include "AttachmentTexture.h"
#include "PostProcessRenderer.h"
#include "Frustum.h"
#include "GBuffer.h"
#include "PBRDeferRenderer.h"
#include "ReflectionProbe.h"
#include "LightProbeRenderer.h"
#include "AABBModule.h"
#include "PrefilterShader.h"
#include "IrradianceShader.h"
#include "ShadowRenderer.h"
#include "FrameBuffer.h"

template<> RenderManager* Singleton<RenderManager>::singleton = nullptr;
RenderManager::RenderManager():
	b_skybox_initialized(false)
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	glGenFramebuffers(1, &capture_fbo);	
	glGenRenderbuffers(1, &capture_rbo);

	win_viewport_info.width = WindowManager::s_current_window->GetWidth();
	win_viewport_info.height = WindowManager::s_current_window->GetHeight();

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


	//brdf_lut->data_format = GL_RG;



	glGenBuffers(1, &camera_ubo);	// binding point is 0	//? 写成一个类
	glObjectLabel(GL_UNIFORM_BUFFER, camera_ubo, -1, "camera_ubo");
	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	ViewportInfo viewport_info{ win_viewport_info.width, win_viewport_info.height };
	glGenBuffers(1, &viewport_ubo);	// binding point is 1	
	glObjectLabel(GL_BUFFER, viewport_ubo, -1, "viewport_ubo");
	glBindBuffer(GL_UNIFORM_BUFFER, viewport_ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, viewport_ubo);
	ModifyCurrentViewportInfo(viewport_info);


	glGenBuffers(1, &light_ssbo);
	glObjectLabel(GL_BUFFER, light_ssbo, -1, "light_ssbo");
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, light_ssbo);	// binding point is 1
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	glGenBuffers(1, &probe_aabb_ssbo);	
	glObjectLabel(GL_SHADER_STORAGE_BUFFER, probe_aabb_ssbo, -1, "probe_aabb_ssbo");
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, probe_aabb_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, probe_aabb_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

//
//
//	glGenFramebuffers(1, &lighting_pass_fbo);
//	glObjectLabel(GL_FRAMEBUFFER, lighting_pass_fbo, -1, "lighting_pass_fbo");
//	glBindFramebuffer(GL_FRAMEBUFFER, lighting_pass_fbo);
//
//	color_tex = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
//	color_tex->internal_format = GL_RGBA;
//	color_tex->data_type = GL_UNSIGNED_BYTE;
//	color_tex->Buffer();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex->id, 0);
//	glObjectLabel(GL_TEXTURE, color_tex->id, -1, "color_tex");
//#ifdef DEBUG_MODE
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "ERROR<RenderManager> Lighting Pass Frame buffer not complete!" << std::endl;
//#endif
//	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());

/*G-Buffer*/
	g_buffer = make_shared<GBuffer>(viewport_info.width, viewport_info.height);
	cur_g_buffer = g_buffer;
	
//	glGenFramebuffers(1, &gbuffer_fbo);
//	glObjectLabel(GL_FRAMEBUFFER, gbuffer_fbo, -1, "gbuffer_fbo");
//	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
//	
//	position_g_attachment = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, true));	//因为需要采样数据，所以position等使用纹理附件而不是渲染缓冲对象附件
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_g_attachment->id, 0);	//附加纹理到FBO中
//	glObjectLabel(GL_TEXTURE, position_g_attachment->id, -1, "position_g_attachment");
//
//	normal_g_attachment = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
//	//normal_g_attachment->wrap_param = GL_REPEAT;
//	normal_g_attachment->Buffer();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_g_attachment->id, 0);
//	glObjectLabel(GL_TEXTURE, normal_g_attachment->id, -1, "normal_g_attachment");
//
//	color_g_attachment = dynamic_cast<AttachmentTexture*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ATTACHMENT, false));
//	color_g_attachment->internal_format = GL_RGBA;
//	color_g_attachment->data_type = GL_UNSIGNED_BYTE;
//	//normal_g_attachment->wrap_param = GL_REPEAT;
//	color_g_attachment->Buffer();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, color_g_attachment->id, 0);
//	glObjectLabel(GL_TEXTURE, color_g_attachment->id, -1, "color_g_attachment");
//
//
//
//	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
//	GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(3, attachments);
//
//	glGenRenderbuffers(1, &gbuffer_depth_rbo);
//	glObjectLabel(GL_RENDERBUFFER, gbuffer_depth_rbo, -1, "gbuffer_depth_rbo");
//	glBindRenderbuffer(GL_RENDERBUFFER, gbuffer_depth_rbo);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, win_width, win_height);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gbuffer_depth_rbo);
//#ifdef DEBUG_MODE
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "ERROR<RenderManager>: Frame buffer not complete!" << std::endl;
//#endif
//	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());


	channel_combination_shader = new ChannelCombinationShader();

	post_process_renderer = new PostProcessRenderer(viewport_info.width, viewport_info.height);

	skybox_shader = new SkyboxShader();

	prefilter_shader = make_shared<PrefilterShader>();

	irradiance_shader = make_shared<IrradianceShader>();
}

bool RenderManager::Initialize(Renderer* renderer_)
{

	if (!b_initialized)
	{
		if (renderer_ == nullptr)
		{
			renderer_ = new BasicRenderer();
		}

		if (b_defer_rendering)
		{
			/*Defer Rendering*/
			pbr_defer_renderer = make_unique<PBRDeferRenderer>();
		}
		else
		{
			current_renderer = renderer_;
		}
		b_initialized = true;

		basic_renderer = new BasicRenderer();

		shadow_renderer = new ShadowRenderer(shadow_far_plane);
	

		light_probe_renderer = new LightProbeRenderer(LightProbe::s_capture_width, LightProbe::s_capture_height);
	}

	return b_initialized;
}

void RenderManager::PreRender()
{
	//UpdateLightArray();
	//for (auto& point_light : SceneManager::GetSingletonPtr()->point_light_array)
	//{
	//	shadow_renderer->DrawDepthMap(point_light);
	//}

	for (auto& probe : SceneManager::GetSingletonPtr()->reflection_probe_set)
	{
		//reflection_probe = new ReflectionProbe({ 0, 0, 0 }, AABBModule());
		light_probe_renderer->Render(probe);	//? 可能是framebuffer绑定错误
	}
	//light_probe_renderer->Render(reflection_probe);

	b_prerendered = true;

}

void RenderManager::Render(float dt)
{
	//UpdateLightArray();
	////UpdateEnvironmentLight();

	//for (auto& point_light : SceneManager::GetSingletonPtr()->point_light_array)
	//{
	//	shadow_renderer->DrawDepthMap(point_light);
	//}

	//glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pbr_defer_renderer->Update(dt);

	////post_process_renderer->Update(dt);
	//
	////ResetRenderArray();
}


void RenderManager::Update(float dt)
{
	UpdateLightArray();

	UpdateLightArray();

	for (auto& point_light : SceneManager::GetSingletonPtr()->point_light_array)	//? 移到PreRender
	{
		shadow_renderer->DrawDepthMap(point_light);
	}


	if (!b_prerendered)
	{
		//PreRender();
		PreRender();
	}

	


	UpdateEnvironmentLight();

	camera = SceneManager::GetSingleton().main_camera;
	CameraInfo camera_info;
	camera_info.position = { camera->GetPosition(), 0 };
	camera_info.view = camera->ViewMatrix();
	camera_info.projection = camera->PerspectiveMatrix();
	ModifyCurrentCameraInfo(camera_info);


	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (b_defer_rendering)
	{
		pbr_defer_renderer->Update(dt);

	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gbuffer_fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current_renderer->Update(dt);

		basic_renderer->Update(dt);

		skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);

		glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	}
	
	post_process_renderer->Update(dt);


	//if (!b_prerendered)
	//{
	//	//PreRender();
	//	//skybox_cubemap = reflection_probe->cubemap;
	//}
	//light_probe_renderer->Render(reflection_probe);

	ResetRenderArray();
}




void RenderManager::UpdateCamera()
{

	camera = SceneManager::GetSingleton().main_camera;

	CameraInfo camera_info;
	camera_info.position = { camera->GetPosition(), 0 };
	camera_info.view = camera->ViewMatrix();
	camera_info.projection = camera->PerspectiveMatrix();

	ModifyCurrentCameraInfo(camera_info);
}

void RenderManager::ModifyCurrentCameraInfo(const CameraInfo& camera_info)
{
	cur_camera_info = camera_info;
	glBindBuffer(GL_UNIFORM_BUFFER, camera_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraInfo), &camera_info, GL_STREAM_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



void RenderManager::UpdateLightArray()
{

	vector<PointLightInfo> light_info_array;
	for (const auto& light : SceneManager::GetSingleton().point_light_array)
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
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLightInfo) * light_info_array.size(), &light_info_array[0], GL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


}

void RenderManager::UpdateEnvironmentLight()
{
	//irradiance_cubemaps.clear();
	//prefilter_cubemaps.clear();
	enabled_probes.clear();

	vector<ProbeAABBInfo> infos;
	for (const auto& probe : SceneManager::GetSingleton().reflection_probe_set)
	{
		//irradiance_cubemaps.emplace_back(probe->irradiance_cubemap);
		//prefilter_cubemaps.emplace_back(probe->prefilter_cubemap);
		enabled_probes.emplace_back(probe);

		ProbeAABBInfo info;
		info.probe_pos = { probe->GetWorldTransform().GetPosition(), 1 };
		info.aabb_pos = { probe->aabb_module->GetWorldPosition(), 1 };
		info.half_dimension = { probe->aabb_module->volume.half_size, 1 };
		infos.emplace_back(info);
	}

	//ReflectionProbe* probe = *SceneManager::GetSingleton().reflection_probe_set.begin();
	//blended_irradiance_cubemap = probe->irradiance_cubemap;
	//blended_prefilter_cubemap = probe->prefilter_cubemap;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, probe_aabb_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ProbeAABBInfo) * infos.size(), &infos[0], GL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	//ModifyProbeAABBInfo(infos);
}


void RenderManager::ResetRenderArray()
{
	pbr_mat_unit_map.clear();
}

bool RenderManager::InsertRenderUnit(RenderUnit* ru)
{
	Material* material = ru->GetMaterial();

	Camera* camera = SceneManager::GetSingleton().main_camera;
	//vec3 camera_to_ru = camera->GetPosition() - ru->GetParent()->GetWorldTransform().GetPosition();
	//ru->SetCameraZDistance(dot(camera_to_ru, camera->GetForward()));

	ru->SetCameraZDistance((camera->ViewMatrix() * vec4(ru->GetPosition(), 1)).z);

	if (b_frustum_culling)
	{
		//vec3 pos = ru->GetPosition();
		//vec3 dimen = ru->GetParent()->GetWorldTransform().GetDimention();
		AABBVolume aabb = ru->GetAABBVolume();
		if (camera->frustum->AABBIntersection(ru->GetPosition(), ru->GetAABBVolume()))
		{
			//ru->SetCameraZDistance(dot(camera_to_ru, camera->GetForward()));
		}
		else
		{
			return false;
		}
	}

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
			pbr_mat_unit_map.emplace(pbr_mat, multiset<RenderUnit*, RenderUnit::CameraZDistanceComp>());
		}
		pbr_mat_unit_map[pbr_mat].emplace(ru);
	}

	return true;
}

void RenderManager::DrawCaptureCubeMesh(Shader* shader)
{
	glDisable(GL_CULL_FACE);
	capture_cube_mesh->Draw(shader);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
}

void RenderManager::DrawCaptureQuadMesh(Shader* shader)
{
	glDisable(GL_CULL_FACE);
	capture_quad_mesh->Draw(shader);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
}

void RenderManager::BindSkyboxTexture(HDRTextureFile* hdr_file)
{
	if (!b_skybox_initialized)
	{
		skybox_cubemap = ResourceManager::GetSingleton().CreateCubemap(512, 512, TextureType::CUBEMAP);
		skybox_cubemap->b_genarate_mipmap = false;
		skybox_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
		skybox_cubemap->data_type = GL_FLOAT;
		skybox_cubemap->Buffer();
	}

	//HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/old_hall.hdr"), TextureFileType::HDR));
	EquirectangularMap* equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::EQUIRECTANGULARMAP, true, hdr_file));	//? 先删除旧的
	//equirectanguler_map->Buffer();

	HDRIShader hdri_cubemap_shader;
	hdri_cubemap_shader.RenderCubeMap(skybox_cubemap, equirectanguler_map);

}

void RenderManager::CombineChannels(PlaneTexture* out_tex, PlaneTexture* tex1, PlaneTexture* tex2)
{
	channel_combination_shader->RenderTexture(out_tex, tex1, tex2);
}

void RenderManager::ModifyCurrentViewportInfo(const ViewportInfo& info)
{

	cur_viewport_info = info;
	glViewport(0, 0, info.width, info.height);

	glBindBuffer(GL_UNIFORM_BUFFER, viewport_ubo);
	//glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(int), NULL, GL_STATIC_DRAW);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &viewport_info.width);
	//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int), sizeof(int), &viewport_info.height);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ViewportInfo), &info, GL_DYNAMIC_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

vector<mat4> RenderManager::GetCaptureViewArray(const vec3& pos)
{
	return {
		glm::lookAt(pos, pos + vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(pos, pos + vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(pos, pos + vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(pos, pos + vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(pos, pos + vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(pos, pos + vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
}

void RenderManager::ModifyProbeAABBInfo(const vector<ProbeAABBInfo>& infos)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, probe_aabb_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ProbeAABBInfo) * infos.size(), &infos[0], GL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}



void RenderManager::DrawMeshes(MVPShader* shader)
{
	auto comp = [](const std::pair<float, PBRMaterial*>& a, const std::pair<float, PBRMaterial*>& b) {return a.first < b.first; };	//大顶堆，因为先渲染cameraZ值大的（靠前的）

	priority_queue<std::pair<float, PBRMaterial*>, vector<std::pair<float, PBRMaterial*>>, decltype(comp)> heap(comp);

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
	{
		heap.emplace((*pair.second.begin())->GetCameraZDistance(), pair.first);
	}


	while (!heap.empty())
	{
		const auto& material = heap.top().second;
		//pbr_geometry_pass_shader->BindMaterial(material);

		const auto& ru_set = RenderManager::GetSingleton().pbr_mat_unit_map[material];
		for (const auto& render_unit : ru_set)
		{
			Mesh* mesh = render_unit->GetMesh();
			mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();
			shader->SetModelMatrix(model);
			mesh->Draw(shader);
		}
		heap.pop();
	}
}

void RenderManager::BindFrameBuffer(FrameBuffer* fb)
{
	if (cur_framebuffer == fb)
	{
		return;
	}

	if (cur_framebuffer->width != cur_viewport_info.width || cur_framebuffer->height != cur_viewport_info.width)
	{
		glViewport(0, 0, fb->width, fb->height);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, fb->id);

	cur_framebuffer = fb;
}


