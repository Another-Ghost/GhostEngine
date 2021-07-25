#pragma once
#include "Renderer.h"
class PBRRenderer : public Renderer
{
public:

	PBRLightingPassShader* pbr_shader;

	SSAOBlurShader* ssao_blur_shader;


	CubeMap* irradiance_cubemap;

	CubeMap* prefilter_cubemap;

	PBRRenderer();

	void Update(float dt) override;

	void OnKeyPressed(Window* window) override;

private:
	static bool RenderUnitCompare(RenderUnit* ru_a, RenderUnit* ru_b);

};

