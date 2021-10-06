#include "CaptureFrameBuffer.h"

CaptureFrameBuffer::CaptureFrameBuffer()
	:FrameBuffer(0, 0)
{
	depth_rbo = make_unique<RenderBuffer>();
}

void CaptureFrameBuffer::Bind(const PlaneTexture* tex)
{
	Bind(tex->width, tex->height);
	AttachTexture2D(tex);
}

void CaptureFrameBuffer::Bind(const CubeMap* cube_map)
{
	Bind(cube_map->width, cube_map->height);
	AttachCubeMap(cube_map);
}

void CaptureFrameBuffer::Bind(int _w, int _h)
{
	width = _w;
	height = _h;
	Bind();

	if (depth_rbo->width != _w || depth_rbo->height != _h)
	{
		depth_rbo->Resize(_w, _h);
	}
	color_attachment_index = 0;
}



