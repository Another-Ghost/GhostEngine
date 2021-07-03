#include "PBRMaterialFactory.h"
#include "PBRMaterial.h"

Material* PBRMaterialFactory::CreateMaterial() const
{
    return new PBRMaterial();
}
