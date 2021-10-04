#pragma once
#include "BasicDependencies.h"

struct FrameBuffer
{
	unsigned int id;

	int width;
	int height;

	virtual void Bind();

	static FrameBuffer* Generate();

	//static vector<FrameBuffer*> frame_buffers;
	//static stack<FrameBuffer*> fbo_stack;

	//static FrameBuffer* cur_buffer;

};

