#include "quasi23.h"
#include "../../kipod/src/modules/meshmodels/meshmodel_primitive.h"
namespace quacry{
using namespace kipod::MeshModels;
auto MakeGenericQuasi23(float scale) -> Quasicrystal23
{
    auto window = PrimMeshModel(Primitive::Cube);
    window.local_->Scale(glm::vec3(scale));
    return {"Cubic Window", Mat5f::Identity() , window , {-4,4, -4,4,-4,4,-4,4,-4,4}};
}
}
