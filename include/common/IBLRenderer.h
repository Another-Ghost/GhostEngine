#pragma once
#include "Renderer.h"
class IBLRenderer : public Renderer
{
public:
	IBLRenderer();

	void Update(float dt) override;

private:
	Shader* pbr_shader;
	Shader* irradiance_shader;
	Shader* background_shader;
	Shader* equirectangular_cubemap_shader;

	unsigned int capture_fbo;
	unsigned int capture_rbo;

	CubeMap* env_cubemap;

	CubeMap* irradiance_cubemap;

	CubeGeometryMesh* cube_mesh;	//改为从ResourceManager中获取
};