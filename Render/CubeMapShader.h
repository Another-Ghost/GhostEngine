#pragma once
#include "MVPShader.h"

class CubeMapShader : public MVPShader
{
public:
	CubeMapShader(const string& vertex_path = File::GetShaderPath("cubemap_vs"), const string& fragment_path = File::GetShaderPath("cubemap_fs"), const string& geometry_path = "");

	void RenderCubeMap(const CubeMap* cube_map);	//? 提取出cubemapshader基类

	void SetColor(const vec3& color_);

private:
	vec3 color = { 1.f, 1.f, 1.f };
};

