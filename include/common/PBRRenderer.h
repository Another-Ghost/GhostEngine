#pragma once
#include "Renderer.h"
class PBRRenderer : public Renderer
{
public:
	PBRRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

	PBRLightingPassShader* pbr_shader;

	SSAOBlurShader* ssao_blur_shader;

	SkyboxShader* skybox_shader;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

};

