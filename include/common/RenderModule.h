#pragma once
#include "Module.h"

class RenderModule : public Module
{
protected:
	RenderModule* parent;

	Mesh* mesh;

	Material* material;

public:
	RenderModule() : b_rendered(true) {}

	Transform transform;

	virtual void Update(float dt) override;

	virtual RenderModule* GetParent() { return parent; }
	virtual void SetParent(RenderModule* parent_) { parent = parent_; }

	Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* mesh_) { mesh = mesh_; }

	Material* GetMaterial() { return material; }
	void SetMaterial(Material* material_) { material = material_; }

	bool b_rendered;
};

