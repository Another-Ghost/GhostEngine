#pragma once
#include "Mesh.h"



class GeometryMesh : public Mesh
{
public:
	GeometryMesh();

	virtual void Initialize();

	virtual void Buffer() override;

	virtual void Draw() override;

	bool b_initialized;

protected:

	unsigned int vbo_id;
	unsigned int ebo_id;


};

