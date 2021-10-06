#pragma once
#include "BasicDependencies.h"
#include "RenderBuffer.h"
#include "AttachmentTexture.h"
#include "CubeMap.h"

class FrameBuffer;
struct FBOInfo
{
	FrameBuffer* fbo;
	int width;
	int height;

	FBOInfo(FrameBuffer* _fbo, int _w, int _h)
		:fbo(_fbo), width(_w), height(_h) {}
};

class FrameBuffer
{
public:
	FrameBuffer(int _w, int _h);

	FrameBuffer(int _id, int _w, int _h);

	unsigned int id;

	int width;
	int height;

	unique_ptr<RenderBuffer> depth_rbo;

	virtual void Bind();

	virtual void Unbind();

	void GenerateDepthRBO();

	void AttachTexture2D(const PlaneTexture* tex);

	void AttachCubeMap(const CubeMap* cube_map);

	void SpecifyDrawColorBuffers();

	bool CheckStatus();

public:
	static shared_ptr<FrameBuffer> screen_fbo;	//? 外部初始化（window）
	//static vector<FrameBuffer*> frame_buffers;

protected:
	int color_attachment_index{ 0 };

private:
	static stack<FBOInfo> fbo_stack;

	static FBOInfo cur_fbo_info;

	static void UpdateCurrentFBOInfo(FBOInfo info);

	static const GLenum color_attachments[16]; //? 改为32

	/*debug*/
	static bool b_nest_bind;
};

