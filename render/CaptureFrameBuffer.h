#pragma once
#include "FrameBuffer.h"
class CaptureFrameBuffer : public FrameBuffer
{
public:
	CaptureFrameBuffer();

	void Bind(const PlaneTexture* tex);

	void Bind(const CubeMap* cube_map);

	void Bind(int _w, int _h);

//private:
	void Bind() override { 
		FrameBuffer::Bind(); 
	}
	//void Unbind();
};

