#include "CubeMapShader.h"
#include "CubeMap.h"
#include "RenderManager.h"


CubeMapShader::CubeMapShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	Use();
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjectionMatrix());
	SetColor(color);
}

void CubeMapShader::RenderCubeMap(const CubeMap* cube_map)
{

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCaptureFBO());
	glBindRenderbuffer(GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cube_map->width, cube_map->height);	//����һ�������Ⱦ�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RenderManager::GetSingleton().GetCaptureRBO());	//����һ�������Ⱦ������󵽵�ǰ�󶨵�֡���������

	GLCall(glViewport(0, 0, cube_map->width, cube_map->height));

	const vector<mat4>& capture_view_array = RenderManager::GetSingleton().GetCaptureViewArray();
	for (int i = 0; i < 6; ++i)
	{
		SetViewMatrix(capture_view_array[i]);
		//SetMat4("view", capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map->id, 0);	//��ɫ��Ⱦ���������������Ⱦ��֮ǰ�����������Ⱦ�������(RBO)��
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderManager::GetSingleton().DrawCaptureCubeMesh(this); //? �Ƿ����������е�һ��һ����Ⱦ����cubemap
	}

	glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
	glViewport(0, 0, RenderManager::GetSingleton().GetCurrentViewportInfo().width, RenderManager::GetSingleton().GetCurrentViewportInfo().height);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map->id);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);	//������Ⱦ֮������mipmap
}

void CubeMapShader::SetColor(const vec3& color_)
{
	Use();
	color = color_; 
	SetVec3("color", color);
}
