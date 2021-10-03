#pragma once
#include "MeshFactory.h"
#include "TriangleMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class TriangleMeshFactory : public MeshFactory
{
	Mesh* CreateMesh() const override;
	 
};

