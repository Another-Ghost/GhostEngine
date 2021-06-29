#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "CubeGeometryMesh.h"
#include "QuadGeometryMesh.h"


//struct RenderMaterialComp
//{
//	bool operator()(RenderModule* a, RenderModule* b)
//	{
//		Unit* parent_a = a->GetParent();
//		Unit* parent_b = b->GetParent();
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

class RenderManager : public Singleton<RenderManager>
{
public:

	Camera* camera;

	//map<Material*, set<RenderModule*>> mat_module_map;
	map<PBRMaterial*, set<RenderModule*>> pbr_mat_module_map;

	RenderManager();

	bool Initialize(Renderer* renderer_ = nullptr);
	
	void Update(float dt);

	void RenderPBRMaterial(float dt);

	void ResetRenderArray();

	void InsertRenderModule(RenderModule* rm);

	unsigned int GetCaptureFBO() { return capture_fbo; }
	unsigned int GetCaptureRBO() { return capture_rbo; }

	const CubeGeometryMesh* GetCaptureCubeMesh() { return capture_cube_mesh; }	//底层const，指向常量的指针

	void DrawCaptureCubeMesh() { capture_cube_mesh->Draw(); }

	void DrawCaptureQuadMesh() { capture_quad_mesh->Draw(); }

	const vector<mat4>& GetCaptureViewArray() { return capture_view_array; }

	const mat4& GetCaptureProjectionMatrix() { return capture_projection; }

	void BindSkyboxTexture(HDRTextureFile* hdr_file);

	CubeMap* GetSkybox() { return skybox_cubemap; }

	Texture* GetBRDFLUT() { return brdf_lut; }

private:
	Shader* pbr_shader;

	Shader* current_shader;

	//set<RenderModule*> set_map;
	//render_module_group

	friend class SceneManager;

	mat4 model_matrix;
	mat4 view_matrix;
	mat4 perspective_matrix;

	Renderer* current_renderer;

	unsigned int capture_fbo;
	unsigned int capture_rbo;
	vector<mat4> capture_view_array;
	mat4 capture_projection;
	CubeGeometryMesh* capture_cube_mesh;

	QuadGeometryMesh* capture_quad_mesh;

	friend class RenderUnit;

	bool b_initialized = false;

	CubeMap* skybox_cubemap;

	bool b_skybox_initialized;

	Texture* brdf_lut;
};

