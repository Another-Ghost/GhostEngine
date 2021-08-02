#include "PointShadowDepthShader.h"
#include "RenderManager.h"
#include "CubeMap.h"
#include "PointLight.h"

PointShadowDepthShader::PointShadowDepthShader(const string& vertex_path /*= File::GetShaderPath("point_shadow_depth_v")*/, const string& fragment_path /*= File::GetShaderPath("point_shadow_depth_f")*/, const string& geometry_path /*= "point_shadow_depth_g"*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void PointShadowDepthShader::Initialize(bool b_reload)
{
	//SetFloat("far_plane", far_plane);

}

void PointShadowDepthShader::SetShadowMatrix(const vector<mat4>& shadow_matrices)
{
	for (int i = 0; i < 6; ++i)
	{
		SetMat4("shadow_matrices[" + std::to_string(i) + "]", shadow_matrices[i]);
	}
}

void PointShadowDepthShader::Draw(CubeMap* depth_cube_map, PointLight* light)
{
	//vector<mat4> view_matrices = RenderManager::GetSingletonPtr()->GetCaptureViewArray(light->GetPosition());

	//for (int i = 0; i < 6; ++i)
	//{
	//	shadow_transforms.emplace_back(shadow_projection * view_matrices[i]);
	//}

	//glViewport(0, 0, shadow_width, shadow_height);
	//glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	//glClear(GL_DEPTH_BUFFER_BIT);


	//point_shadow_depth_shader->


	//	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	//glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);
}

