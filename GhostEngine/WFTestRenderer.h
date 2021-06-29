#pragma once
#include "common/Renderer.h"
#include "WFTestShader.h"

class WFTestRenderer : public Renderer
{
public:
	WFTestRenderer();

	void Update(float dt) override;

private:
	WFTestShader* pbr_shader;
	SkyboxShader* skybox_shader;

	unsigned int capture_fbo;
	unsigned int capture_rbo;

	//CubeMap* env_cubemap;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	//Texture* brdf_Lut_texture;
	Texture* brdf_lut_texture;

	Texture* ssda_lut_texture;

	//EquirectangularMap* equirectanguler_map;
};

