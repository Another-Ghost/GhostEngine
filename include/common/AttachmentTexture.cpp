#include "AttachmentTexture.h"
#include "RenderManager.h"

AttachmentTexture::AttachmentTexture():
	PlaneTexture(TextureType::ATTACHMENT)
{
	width = RenderManager::GetSingleton().GetViewportInfo().width;
	height = RenderManager::GetSingleton().GetViewportInfo().height;
	data_format = GL_RGBA;
	min_filter_param = GL_NEAREST;
	mag_filter_param = GL_NEAREST;
	data_type = GL_FLOAT;
	wrap_param = GL_CLAMP_TO_EDGE;
}
