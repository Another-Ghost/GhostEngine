#include "SSAOBlurShader.h"


SSAOBlurShader::SSAOBlurShader(const string& vertex_path, const string& fragment_path, const string& geometry_path)
	:Shader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void SSAOBlurShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::ssao);

	if (b_reload)
	{
		Shader::Initialize(b_reload);
	}
}


