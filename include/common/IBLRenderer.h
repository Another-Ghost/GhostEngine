#pragma once

#include "Renderer.h"
class IBLRenderer : public Renderer
{
public:
	IBLRenderer();

	void Update(float dt) override;

private:
	PBRShader* pbr_shader;
	Shader* irradiance_shader;
	Shader* background_shader;
	Shader* equirectangular_cubemap_shader;
	Shader* prefilter_shader;
	Shader* brdf_shader;

	unsigned int capture_fbo;
	unsigned int capture_rbo;

	CubeMap* env_cubemap;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	//Texture* brdf_Lut_texture;
	Texture* brdf_lut_texture;

	CubeGeometryMesh* cube_mesh;	//改为从ResourceManager中获取
	
	EquirectangularMap* equirectanguler_map;

};