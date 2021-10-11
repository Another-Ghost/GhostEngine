#include "FBOShader.h"

FBOShader::FBOShader(const string& vertex_path /*= File::GetShaderPath("basic_v")*/, const string& fragment_path /*= File::GetShaderPath("fbo_f")*/, const string& geometry_path /*= ""*/)
	: Shader(vertex_path, fragment_path, geometry_path)
{
	//UseTextureUnit(TextureUnit::g_color);
	UseTextureUnit(TextureUnit::texture_0);
}

void FBOShader::Draw(PlaneTexture* tex, shared_ptr<FrameBuffer> fbo)
{
	TextureUnit::Bind2DTexture(TextureUnit::texture_0, tex);

	fbo->Bind();	//? 提炼为fbo的函数
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderManager::GetSingleton().DrawCaptureCubeMesh(this);
	fbo->Unbind();
}
