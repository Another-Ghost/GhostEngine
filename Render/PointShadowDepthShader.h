#pragma once
#include "MVPShader.h"

class PointShadowDepthShader : public MVPShader
{
public:

	//float  near_plane{ 1 };
	//float far_plane{ 25 };

	PointShadowDepthShader(const string& vertex_path = File::GetShaderPath("point_shadow_depth_v"), const string& fragment_path = File::GetShaderPath("point_shadow_depth_f"), const string& geometry_path = File::GetShaderPath("point_shadow_depth_g"));

	void Initialize(bool b_reload) override;

	void SetShadowMatrix(const vector<mat4>& shadow_matrices); 

	void Draw(CubeMap* depth_cube_map, PointLight* light);

};

