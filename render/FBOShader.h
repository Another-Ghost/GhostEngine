#pragma once
#include "Shader.h"
class FBOShader :
    public Shader
{
public:
	FBOShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("fbo_f"), const string& geometry_path = "");

	void Draw(PlaneTexture* tex, shared_ptr<FrameBuffer> fbo);
};

