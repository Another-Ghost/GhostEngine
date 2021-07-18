#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "CubeGeometryMesh.h"
#include "QuadGeometryMesh.h"


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

struct WindowInfo
{
	int width;
	int height;
};

class RenderManager : public Singleton<RenderManager>
{
public:

	Camera* camera;

	//map<Material*, set<RenderModule*>> mat_module_map;
	map<PBRMaterial*, set<RenderUnit*>> pbr_mat_unit_map;
	map<BasicMaterial*, set<RenderUnit*>> basic_mat_unit_map;

	RenderManager();

	bool Initialize(Renderer* renderer_ = nullptr);
	
	void Update(float dt);

	void UpdateCamera();

	void UpdateLightArray();

	void UpdateSSAO();

	void ResetRenderArray();

	void InsertRenderUnit(RenderUnit* ru);

	unsigned int GetCaptureFBO() { return capture_fbo; }
	unsigned int GetCaptureRBO() { return capture_rbo; }

	const CubeGeometryMesh* GetCaptureCubeMesh() { return capture_cube_mesh; }	//底层const，指向常量的指针

	void DrawCaptureCubeMesh(Shader* shader) { capture_cube_mesh->Draw(shader); }

	void DrawCaptureQuadMesh(Shader* shader) { capture_quad_mesh->Draw(shader); }

	const vector<mat4>& GetCaptureViewArray() { return capture_view_array; }

	const mat4& GetCaptureProjectionMatrix() { return capture_projection; }

	void BindSkyboxTexture(HDRTextureFile* hdr_file);

	CubeMap* GetSkybox() { return skybox_cubemap; }

	Texture* GetBRDFLUT() { return brdf_lut; }

	GLuint GetLightSSBO() { return light_ssbo; }
	//GLuint GetLightColorSSBO() { return light_color_ssbo; }

	void CombineChannels(Texture* out_tex, Texture* tex1, Texture* tex2);

private:

	Shader* current_shader;

	Shader* pbr_shader;

	ChannelCombinationShader* channel_combination_shader;

	SSAOShader* ssao_shader;
	SSAOBlurShader* ssao_blur_shader;

	OutputShader* output_shader;

	friend class SceneManager;

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

	Texture* brdf_lut;

	GLuint light_ssbo;

	GLuint ssao_kernel_ssbo;

	GLuint camera_ubo;
	GLuint window_ubo;
	//GLuint light_color_ssbo;

/*Post Process*/
	/*G-buffer*/
	unsigned int gbuffer_fbo;
	unsigned int gbuffer_depth_rbo;	//? 是否可以使用capture_fbo

	Texture* position_g_attachment;
	Texture* normal_g_attachment;
	Texture* color_g_attachment;

	/*SSAO*/
	unsigned int ssao_fbo;
	unsigned int ssao_blur_fbo;
	Texture* ssao_color_attachment;
	Texture* ssao_blur_color_attachment;

	vector<vec4> ssao_kernel;

	int sample_num;
	Texture* noise_texture;
	int noise_tex_size{ 4 };
};

