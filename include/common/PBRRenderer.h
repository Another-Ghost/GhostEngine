#pragma once
#include "Renderer.h"
class PBRRenderer : public Renderer
{
public:

	PBRShader* pbr_shader;

	SSAOBlurShader* ssao_blur_shader;

	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PlaneTexture* brdf_lut;

	PBRRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

private:
	static bool RenderUnitCompare(RenderUnit* ru_a, RenderUnit* ru_b);

};

