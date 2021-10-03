#include "SphereGeometryMeshFactory.h"
#include "SphereGeometryMesh.h"

SphereGeometryMeshFactory::SphereGeometryMeshFactory()
{

}

Mesh* SphereGeometryMeshFactory::CreateMesh() const
{
    return new SphereGeometryMesh();
}
