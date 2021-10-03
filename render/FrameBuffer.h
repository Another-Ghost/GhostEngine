#pragma once
struct FrameBuffer
{
	unsigned int id;

	int width;
	int height;

	virtual void Bind();

	//virtual void Unbind();
};

