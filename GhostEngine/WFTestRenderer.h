#pragma once
#include "common/Renderer.h"
#include "WFTestShader.h"

class WFTestRenderer : public Renderer
{
public:
	WFTestRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

private:
	WFTestShader* pbr_shader;
	SkyboxShader* skybox_shader;

	unsigned int capture_fbo;
	unsigned int capture_rbo;

	//CubeMap* env_cubemap;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PlaneTexture* brdf_lut;

	PlaneTexture* ssda_lut_texture;

	//EquirectangularMap* equirectanguler_map;
};

