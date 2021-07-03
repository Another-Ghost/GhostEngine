#pragma once
#include "Prerequisite.h"


class RenderUnit
{
protected:

	Mesh* mesh;

	Material* material;	//? 给一个默认material和mesh

	RenderModule* parent;

	Transform transform;

	bool b_rendered;

public:

	RenderUnit(RenderModule* parent_ = nullptr, Mesh* mesh_ = nullptr, Material* material_ = nullptr): mesh(mesh_), material(material_), b_rendered(true), parent(parent_) {}

	Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* mesh_) { mesh = mesh_; }

	Material* GetMaterial() { return material; }
	void SetMaterial(Material* material_) { material = material_; }

	virtual RenderModule* GetParent() { return parent; }
	virtual void SetParent(RenderModule* parent_) { parent = parent_; }

	void Update(float dt);
};

