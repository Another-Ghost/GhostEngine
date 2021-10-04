#pragma once
#include "BasicDependencies.h"
#include "RenderBuffer.h"

struct FrameBuffer
{
public:
	FrameBuffer(int _w, int _h);

	FrameBuffer(int _id, int _w, int _h);

	unsigned int id;

	int width;
	int height;

	unique_ptr<RenderBuffer> depth_rbo;

	void Bind();

	void Unbind();

	void GenerateDepthRBO();

public:
	static FrameBuffer* screen_fbo;	//? 外部初始化（window）
	//static vector<FrameBuffer*> frame_buffers;

private:
	static stack<FrameBuffer*> fbo_stack;

	static FrameBuffer* cur_fbo;

	static void BindCurrentFBO(FrameBuffer* _cur_fbo);

};

