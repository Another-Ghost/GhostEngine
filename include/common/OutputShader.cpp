#include "OutputShader.h"

OutputShader::OutputShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	Shader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void OutputShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::g_color);	//?
	UseTextureUnit(TextureUnit::ssao);	

	if (b_reload)
	{
		Shader::Initialize();
	}
}
