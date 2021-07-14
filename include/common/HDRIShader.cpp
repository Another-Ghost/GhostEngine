#include "HDRIShader.h"
#include "EquirectangularMap.h"
#include "RenderManager.h"
#include "WindowManager.h"
#include "CubeMap.h"

HDRIShader::HDRIShader():
MVPShader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("equirectangular_to_cubemap_fs"))
{
	Use();
	SetInt("equirectangular_map", 0);
	
	SetProjectionMatrix(RenderManager::GetSingleton().GetCaptureProjectionMatrix());

	
}



void HDRIShader::RenderCubeMap(const CubeMap* cube_map, unsigned int hdr_tex_id)
{
	Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdr_tex_id);

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
		RenderManager::GetSingleton().DrawCaptureCubeMesh(); //? �Ƿ����������е�һ��һ����Ⱦ����cubemap
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::s_current_window->GetWidth(), WindowManager::s_current_window->GetHeight());

	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map->id);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);	//������Ⱦ֮������mipmap(֮ǰ���ɵĻ��޸����ݺ��ʧЧ)

}
