#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "CubeGeometryMesh.h"
#include "QuadGeometryMesh.h"
#include "RenderUnit.h"
#include "PBRDeferRenderer.h"

//struct RenderMaterialComp
//{
//	bool operator()(RenderModule* a, RenderModule* b)
//	{
//		Node* parent_a = a->GetParent();
//		Node* parent_b = b->GetParent();
//		if (parent_a && parent_b)
//		{
//			return parent_a->transform.GetPosition().z > parent_b->transform.GetPosition().z;
//		}
//		else
//		{
//			cout << "ERROR<RenderManager> from MaterialComp: parent is null \n";
//			return false;
//		}
//	}
//};

struct PointLightInfo	// shader用
{
	vec4 position;
	vec4 color;
	//bool b_rendering;
};

struct CameraInfo
{
	vec4 position;
	mat4 view;
	mat4 projection;
};

struct ViewportInfo
{
	int width;
	int height;
};

struct ProbeAABBInfo
{
	vec4 probe_pos;
	vec4 aabb_pos;
	vec4 half_dimension;
};


//enum class FrameBuffer
//{
//	DEFAULT, //默认帧缓存
//	GBUFFER,
//	SSAO,
//	SSAOBLUR,
//};

class RenderManager : public Singleton<RenderManager>
{
public:

	Camera* camera;

	shared_ptr<GBuffer> g_buffer;
	shared_ptr<GBuffer> cur_g_buffer;

	unsigned int cur_output_fbo{ 0 };

	SkyboxShader* skybox_shader;

	map<PBRMaterial*, multiset<RenderUnit*, RenderUnit::CameraZDistanceComp>> pbr_mat_unit_map;
	//map<PBRMaterial*, vector<RenderUnit*>> pbr_mat_unit_map;
	//multimap<PBRMaterial*, RenderUnit*> pbr_mat_unit_map;

	map<BasicMaterial*, set<RenderUnit*>> basic_mat_unit_map;

	const mat4 probe_capture_projection{ glm::perspective(glm::radians(90.f), 1.f, 0.1f, 500.f) };

	shared_ptr<PrefilterShader> prefilter_shader;
	shared_ptr<IrradianceShader> irradiance_shader;
	
	CubeMap* blended_irradiance_cubemap;
	CubeMap* blended_prefilter_cubemap;

	vector<CubeMap*> irradiance_cubemaps;
	vector<CubeMap*> prefilter_cubemaps;

	vector<ReflectionProbe*> enabled_probes;

	bool b_prerendered{ false };

	/*Shadow*/
	ShadowRenderer* shadow_renderer;

	const int point_shadow_width{ 1024 };
	const int point_shadow_height{ 1024 };

	float shadow_far_plane{64};

	/*Defer Rendering*/
	unique_ptr<PBRDeferRenderer> pbr_defer_renderer;

	bool b_defer_rendering{ true };

	RenderManager();

	bool Initialize(Renderer* renderer_ = nullptr);

	void PreRender();
	
	void Render(float dt);

	void Update(float dt);

	void UpdateCamera();

	void ModifyCurrentCameraInfo(const CameraInfo& camera_info);

	void UpdateLightArray();	//? 一个tick只用更新一次

	void UpdateEnvironmentLight();	

	void ResetRenderArray();

	bool InsertRenderUnit(RenderUnit* ru);

	unsigned int GetCaptureFBO() { return capture_fbo; }
	unsigned int GetCaptureRBO() { return capture_rbo; }

	const CubeGeometryMesh* GetCaptureCubeMesh() { return capture_cube_mesh; }	//底层const，指向常量的指针

	void DrawCaptureCubeMesh(Shader* shader);

	void DrawCaptureQuadMesh(Shader* shader); 

	const vector<mat4>& GetCaptureViewArray() { return capture_view_array; }

	const mat4& GetCaptureProjectionMatrix() { return capture_projection; }

	void BindSkyboxTexture(HDRTextureFile* hdr_file);

	CubeMap* GetSkybox() { return skybox_cubemap; }

	//PlaneTexture* GetBRDFLUT() { return brdf_lut; }

	GLuint GetLightSSBO() { return light_ssbo; }
	//GLuint GetLightColorSSBO() { return light_color_ssbo; }

	void CombineChannels(PlaneTexture* out_tex, PlaneTexture* tex1, PlaneTexture* tex2);


	AttachmentTexture* GetGPosition() { return position_g_attachment; }
	AttachmentTexture* GetGNormal() { return normal_g_attachment; }
	AttachmentTexture* GetGColor(){ return color_g_attachment; }


	
	//unsigned int lighting_pass_fbo;
	//AttachmentTexture* color_tex;

	//void BindFrameBuffer(unsigned int fbo){ glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
	//void UnbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer()); }
	ViewportInfo GetWindowViewportInfo() { return win_viewport_info; }

	void ModifyCurrentViewportInfo(const ViewportInfo& info);

	ViewportInfo GetCurrentViewportInfo() { return cur_viewport_info; }
	CameraInfo GetCurrentCameraInfo() { return cur_camera_info; }

	unsigned int GetCurrentOutputFrameBuffer() { return cur_output_fbo; }

	void BindCurrentOutputFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, cur_output_fbo); }

	void SetCurrentOutputFrameBuffer(unsigned int fbo) { cur_output_fbo = fbo; glBindFramebuffer(GL_FRAMEBUFFER, fbo); }

	vector<mat4> GetCaptureViewArray(const vec3& pos);

	void ModifyProbeAABBInfo(const vector<ProbeAABBInfo>& info);

	void DrawMeshes(MVPShader* shader);

	void BindFrameBuffer(FrameBuffer* fb);

private:

	ViewportInfo win_viewport_info;
	ViewportInfo cur_viewport_info;
	CameraInfo cur_camera_info;

	Shader* current_shader;

	Shader* pbr_shader;



	ChannelCombinationShader* channel_combination_shader;


	mat4 model_matrix;
	mat4 view_matrix;
	mat4 perspective_matrix;

	Renderer* current_renderer;

	BasicRenderer* basic_renderer;

	unsigned int capture_fbo;
	unsigned int capture_rbo;
	vector<mat4> capture_view_array;
	mat4 capture_projection;
	CubeGeometryMesh* capture_cube_mesh;

	QuadGeometryMesh* capture_quad_mesh;

	friend class RenderNode;

	bool b_initialized = false;

	CubeMap* skybox_cubemap;

	bool b_skybox_initialized;


	GLuint light_ssbo;


	GLuint camera_ubo;
	GLuint viewport_ubo;


	GLuint probe_aabb_ssbo;
	//GLuint light_color_ssbo;



	/*G-buffer*/
	unsigned int gbuffer_fbo;
	unsigned int gbuffer_depth_rbo;	//? 是否可以使用capture_fbo

	AttachmentTexture* position_g_attachment;
	AttachmentTexture* normal_g_attachment;
	AttachmentTexture* color_g_attachment;
	
	/*Post Process*/
	PostProcessRenderer* post_process_renderer;


	friend class SceneManager;


	ReflectionProbe* reflection_probe;
	LightProbeRenderer* light_probe_renderer;

	FrameBuffer* cur_framebuffer;


	bool b_frustum_culling{ false };

	bool b_enable_local_IBL{ false };
};

