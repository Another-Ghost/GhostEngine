#include "PBRMaterialFactory.h"
#include "PBRMaterial.h"

Material* PBRMaterialFactory::CreateMaterial()
{
    
    return new PBRMaterial();
}
