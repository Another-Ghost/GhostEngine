#include "MeshFactory.h"
#include "Mesh.h"

Mesh* MeshFactory::CreateMesh() const
{
    return new Mesh();
}
