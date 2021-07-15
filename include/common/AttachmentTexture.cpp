#include "AttachmentTexture.h"
#include "WindowManager.h"

AttachmentTexture::AttachmentTexture():
	Texture(TextureType::ATTACHMENT)
{
	width = WindowManager::s_current_window->GetWidth();
	height = WindowManager::s_current_window->GetHeight();
	data_format = GL_RGBA;
	min_filter_param = GL_NEAREST;
	mag_filter_param = GL_NEAREST;
}
