#pragma once
#include "Renderer.h"
#include "PBRShader.h"
#include "PBRLightingPassShader.h"

class PBRDeferRenderer : public Renderer
{
public:
	PBRDeferRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

private:
	unique_ptr<PBRShader> pbr_geometry_pass_shader;
	unique_ptr<PBRLightingPassShader> pbr_lighting_pass_shader;

	unique_ptr<PBRLightingPassShader> lighting_prerender_shader;


	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PlaneTexture* brdf_lut;


};

