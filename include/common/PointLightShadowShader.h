#pragma once
#include "MVPShader.h"
class PointLightShadowShader :
    public MVPShader
{
    PointLightShadowShader(const string& vertex_path = File::GetShaderPath("point_shadow_v"), const string& fragment_path = File::GetShaderPath("point_shadow_f"), const string& geometry_path = "");
};

