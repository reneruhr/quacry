#include "quasi23.h"
#include "../../kipod/src/modules/meshmodels/meshmodel_primitive.h"
namespace quacry{
using namespace kipod::MeshModels;
auto MakeGenericQuasi23() -> Quasicrystal23
{
    return {"Cubic Window", Mat5f::Identity() , PrimMeshModel(Primitive::Cube) , {-4,4, -4,4,-4,4,-4,4,-4,4}};
}
}
