#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &id);
}

//在一个函数里Bind()应与Unbind()成对出现
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

void FrameBuffer::BindCurrentFBO(FrameBuffer* _cur_fbo)
{

	if (cur_fbo->width != _cur_fbo->width || cur_fbo->height != _cur_fbo->height)
	{
		glViewport(0, 0, _cur_fbo->width, _cur_fbo->height);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _cur_fbo->id);
	cur_fbo = _cur_fbo;
}




