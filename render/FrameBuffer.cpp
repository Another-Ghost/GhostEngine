#include "FrameBuffer.h"

stack<FBOInfo> FrameBuffer::fbo_stack;

shared_ptr<FrameBuffer> FrameBuffer::screen_fbo = make_shared<FrameBuffer>(0, 0, 0);

FBOInfo FrameBuffer::cur_fbo_info{0, 0, 0};

bool FrameBuffer::b_nest_bind = false;

const GLenum FrameBuffer::color_attachments[16] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15 };

FrameBuffer::FrameBuffer(int _w, int _h)
	:width(_w), height(_h)
{
	glGenFramebuffers(1, &id);
}

FrameBuffer::FrameBuffer(int _id, int _w, int _h)
	:id(_id), width(_w), height(_h)
{
}

//在一个函数内Bind()应与Unbind()成对出现
//在任何一个return分支前应添加 Unbind();
void FrameBuffer::Bind()
{
	//if (cur_fbo_info.fbo == this && cur_fbo_info.width == width && cur_fbo_info.height == height)
	//{
	//	return;
	//}
//#ifdef DEBUG_MODE
	if (!b_nest_bind)
	{
		if (fbo_stack.size() == 3)
		{
			cout << "CAUTION<FrameBuffer>Bind: deep nest bind start" << endl;
			b_nest_bind = true;
		}
	}


//#endif

	FBOInfo info{ this, width, height };
	fbo_stack.push(info);
	UpdateCurrentFBOInfo(info);
}

void FrameBuffer::Unbind()
{
	//if (fbo_stack.top().fbo == this && fbo_stack.top().width == width && fbo_stack.top().height && height)
	//{
	//	return;
	//}
	
	fbo_stack.pop();
	if (!fbo_stack.empty())
	{
		FBOInfo cur_fbo = fbo_stack.top();
		cur_fbo.fbo->width = cur_fbo.width;
		cur_fbo.fbo->height = cur_fbo.height;
		UpdateCurrentFBOInfo(cur_fbo);
	}

	if(b_nest_bind)
	{
		if (fbo_stack.size() < 2)
		{
			cout << "CAUTION<FrameBuffer>Bind: nest bind end" << endl;
			b_nest_bind = false;
		}
	}

}

void FrameBuffer::GenerateDepthRBO()
{
	depth_rbo = make_unique<RenderBuffer>(width, height);
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo->id);
	Unbind();

//#ifdef DEBUG_MODE
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "ERROR<GBuffer>: G-Buffer not complete!" << std::endl;
//#endif
}

void FrameBuffer::AttachTexture2D(const PlaneTexture* tex)
{
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachments[color_attachment_index], GL_TEXTURE_2D, tex->id, 0);
	//color_attachments.emplace_back(GL_COLOR_ATTACHMENT0 + color_attachment_index);
	++color_attachment_index;	
	Unbind();
}

void FrameBuffer::AttachCubeMap(const CubeMap* cube_map)
{
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachments[color_attachment_index], GL_TEXTURE_CUBE_MAP, cube_map->id, 0);
	//color_attachments.emplace_back(GL_COLOR_ATTACHMENT0 + color_attachment_index);
	++color_attachment_index;
	Unbind();
}

void FrameBuffer::SpecifyDrawColorBuffers()
{
	//GLenum attachments[32];	//? 是否可以直接用vector
	//for (int i = 0; i < color_attachments.size(); ++i)
	//{
	//	attachments[i] = color_attachments[i];
	//}
	Bind();
	glDrawBuffers(color_attachment_index, color_attachments);
	Unbind();
}

bool FrameBuffer::CheckStatus()
{
	Bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR<Framebuffer>: " << id << " not complete!" << std::endl;
		Unbind();
		return false;
	}
	Unbind();
	return true;
}

void FrameBuffer::UpdateCurrentFBOInfo(FBOInfo info)	//？全局获取viewport的方式
{
	if (cur_fbo_info.width != info.width || cur_fbo_info.height != info.height)
	{
		glViewport(0, 0, info.width, info.height);
	}
	if (cur_fbo_info.fbo != info.fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, info.fbo->id);
	}
	cur_fbo_info = info;
}




