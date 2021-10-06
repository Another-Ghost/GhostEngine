#include "RenderBuffer.h"


RenderBuffer::RenderBuffer()
{
	glGenRenderbuffers(1, &id);
}

RenderBuffer::RenderBuffer(int _w, int _h)
	:width(_w), height(_h)
{
	glGenRenderbuffers(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void RenderBuffer::Resize(int _w, int _h)
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _w, _h);
	width = _w;
	height = _h;
}
