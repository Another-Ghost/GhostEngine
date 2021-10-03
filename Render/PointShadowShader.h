#pragma once
#include "MVPShader.h"
class PointShadowShader :
    public MVPShader
{
public:
    PointShadowShader(const string& vertex_path = File::GetShaderPath("point_shadow_v"), const string& fragment_path = File::GetShaderPath("point_shadow_f"), const string& geometry_path = "");
};

