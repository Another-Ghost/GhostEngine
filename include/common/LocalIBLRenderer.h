#pragma once
#include "Renderer.h"


class LocalIBLRenderer : public Renderer
{
public:
	LocalIBLRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

	void RnderGeometryPass();

	void RenderDirectLight();

	void Render();

private:
	shared_ptr<PBRShader> pbr_geometry_pass_shader;
	shared_ptr<PBRLightingPassShader> pbr_lighting_pass_shader;

	shared_ptr<PBRLightingPassShader> lighting_prerender_shader;

	shared_ptr<PBRLightingPassShader> direct_lighting_shader;
	shared_ptr<PBRLightingPassShader> indirect_lighting_shader;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PlaneTexture* brdf_lut;

};

