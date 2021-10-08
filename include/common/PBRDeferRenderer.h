#pragma once
#include "Renderer.h"


class PBRDeferRenderer : public Renderer
{
public:
	PBRDeferRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

	void RnderGeometryPass();
	
	//void RenderDirectLight();

	void Render();

private:
	shared_ptr<PBRShader> pbr_geometry_pass_shader;
	shared_ptr<PBRLightingPassShader> pbr_lighting_pass_shader;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PlaneTexture* brdf_lut;

};

