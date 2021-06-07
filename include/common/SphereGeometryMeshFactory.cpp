#include "SphereGeometryMeshFactory.h"
#include "SphereGeometryMesh.h"

SphereGeometryMeshFactory::SphereGeometryMeshFactory(float radius_):
    radius(radius_)
{

}

Mesh* SphereGeometryMeshFactory::CreateMesh() const
{
    return new SphereGeometryMesh();
}
