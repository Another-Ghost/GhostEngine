#pragma once
#include "Renderer.h"
class ShadowRenderer : public Renderer
{
public:
	float near_plane{ 0.1f };
	float far_plane{ 25.f };

	ShadowRenderer();

	void Update(float dt) override;

	void DrawDepthMap(PointLight* light);

	

private:
	PointShadowDepthShader* point_shadow_depth_shader;
	//unique_ptr<PointShadowShader> point_shadow_shader;

	unsigned int depth_map_fbo;

	vector<CubeMap*> depth_cubemaps;

	const unsigned int shadow_width{ 1024 };
	const unsigned int shadow_height{ 1024 };

	vector<mat4> shadow_matrices;



	mat4 shadow_projection;
};

