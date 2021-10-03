#include "PointShadowShader.h"

PointShadowShader::PointShadowShader(const string& vertex_path /*= File::GetShaderPath("point_shadow_v")*/, const string& fragment_path /*= File::GetShaderPath("point_shadow_f")*/, const string& geometry_path /*= ""*/)
	:MVPShader(vertex_path, fragment_path, geometry_path)
{

}
