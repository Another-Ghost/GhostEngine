#pragma once
#include "Renderer.h"
#include "RenderManager.h"

class LightProbeRenderer : public Renderer
{

public:
	LightProbeRenderer(int width, int height);

	void Render(LightProbe* probe);

private:
	virtual void Update(float dt) override { };



	shared_ptr<GBuffer> g_buffer;

	unique_ptr<PostProcessRenderer> post_process_renderer;

	int	viewport_width;
	int viewport_height;

	CameraInfo camera_info;
};

