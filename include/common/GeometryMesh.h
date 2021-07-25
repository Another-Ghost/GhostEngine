#pragma once
#include "Mesh.h"


class GeometryMesh : public Mesh
{
public:
	GeometryMesh();

	virtual void Initialize() override;	//Template Method

	virtual void InitializeData() {}

	virtual void Buffer() override;

	virtual void Draw(Shader* shader) override;

	bool b_initialized;

};

