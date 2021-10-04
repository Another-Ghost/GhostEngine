#include "FrameBuffer.h"


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
void FrameBuffer::Bind()
{
	if (cur_fbo == this)
	{
		return;
	}

	fbo_stack.push(this);
	BindCurrentFBO(this);
}

void FrameBuffer::Unbind()
{
	BindCurrentFBO(fbo_stack.top());
	fbo_stack.pop();
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

void FrameBuffer::BindCurrentFBO(FrameBuffer* _cur_fbo)
{
	if (cur_fbo->width != _cur_fbo->width || cur_fbo->height != _cur_fbo->height)
	{
		glViewport(0, 0, _cur_fbo->width, _cur_fbo->height);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _cur_fbo->id);
	cur_fbo = _cur_fbo;
}




