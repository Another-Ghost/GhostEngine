#pragma once
#include "common/Renderer.h"

class Camera;

class BasicRenderer : public Renderer
{
public:
	BasicRenderer(World* world_, Window* window_) :Renderer(world_, window_) {
		Initialize();
		BeginRender();
	}

	bool Initialize() override;

	void BeginRender() override;

	void Update(float dt) override;

	

private:
	Shader* basic_shader;

	Camera* camera;

	Scene* basic_scene;

	//texture
	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;

	vec3 light_positions[1];
	vec3 light_colors[1];
	int num_row = 1;
	int num_column = 1;
	float spacing = 2.5;
};

