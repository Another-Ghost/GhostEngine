#pragma once

#include "Mesh.h"
#include "Material.h"
#include "AABBVolume.h"
#include "Transform.h"

class RenderModule;

class RenderUnit
{
protected:

	Mesh* mesh;

	Material* material;	//? 给一个默认material和mesh

	RenderModule* parent;

	Transform transform;

	bool b_rendered;

	float camera_z_distance;

	AABBVolume aabb_volume;

public:

	RenderUnit(RenderModule* parent_ = nullptr, Mesh* mesh_ = nullptr, Material* material_ = nullptr);

	Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* mesh_) { mesh = mesh_; }

	Material* GetMaterial() { return material; }
	void SetMaterial(Material* material_) { material = material_; }

	virtual RenderModule* GetParent() { return parent; }
	virtual void SetParent(RenderModule* parent_) { parent = parent_; } 

	float GetCameraZDistance() const { return camera_z_distance; }
	void SetCameraZDistance(float dist) { camera_z_distance = dist; }

	void Update(float dt);

	AABBVolume GetAABBVolume();
	                                    
	vec3 GetPosition();

	struct CameraZDistanceComp
	{
		bool operator()(const RenderUnit* a, const RenderUnit* b) const
		{
			return a->GetCameraZDistance() > b->GetCameraZDistance();
		}
	};

};

