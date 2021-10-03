#pragma once
#include "Renderer.h"
#include "RenderManager.h"

class LightProbeRenderer : public Renderer
{

public:
	LightProbeRenderer(int width, int height);

	void Render(ReflectionProbe* probe);

private:

	GLuint capture_fbo;
	GLuint capture_rbo;

	shared_ptr<GBuffer> g_buffer;

	unique_ptr<PostProcessRenderer> post_process_renderer;

	int	viewport_width;
	int viewport_height;

	CameraInfo camera_info;

	virtual void Update(float dt) override { };
};

