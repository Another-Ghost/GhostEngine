#include "CubeGeometryMeshFactory.h"


Mesh* CubeGeometryMeshFactory::CreateMesh() const
{
    return new CubeGeometryMesh();
}
