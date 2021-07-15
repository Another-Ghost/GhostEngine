#pragma once
#ifdef IBL_RENDERER

#include "Renderer.h"
class IBLRenderer : public Renderer
{
public:
	IBLRenderer();

	void Update(float dt) override;

private:
	PBRShader* pbr_shader;
	SkyboxShader* skybox_shader;
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

	EquirectangularMap* equirectanguler_map;

};

#endif