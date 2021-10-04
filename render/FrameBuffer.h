#pragma once
#include "BasicDependencies.h"

struct FrameBuffer
{
public:
	FrameBuffer();

	unsigned int id;


	int width;
	int height;

	void Bind();

	void Unbind();

protected:

	//static vector<FrameBuffer*> frame_buffers;

private:
	static stack<FrameBuffer*> fbo_stack;

	static FrameBuffer* cur_fbo;

	static void BindCurrentFBO(FrameBuffer* _cur_fbo);
};

