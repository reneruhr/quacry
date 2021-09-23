#include "kipod.h"

#include "kipod/src/modules/meshmodels/meshmodel_module.h"
#include "src/quasicrystals_module.h"
#include "src/quasicrystals_examples.h"

extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;

using namespace kipod;
using namespace quacry;
using namespace kipod::MeshModels;

int main( int argc, char **argv )
{
//    int width = 1024, height = 768;
    //int width = 512, height = 384;

    int width = 800, height = 800;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(QuasiCrystalsModule("Quasicrystals", width, height));
    kipod.Add(MeshModelModule("Meshmodels", width,height));

    auto quacry = kipod.GetModule("Quasicrystals");
    auto scene = static_cast<QuasiCrystalsModule*>(quacry)->GetScene();
    auto menu = static_cast<QuasiCrystalsModule*>(quacry)->GetScene();

    std::cout << PenroseRotation();

    using Mat5f = Eigen::Matrix<float,5,5>;
    using Vec5f = Eigen::Matrix<float,5,1>;
    auto [v,i] = PenroseInternalPolytope(Mat5f::Identity(), Vec5f::Constant(0.5f));
    std::cout << "Vertices count: " << v.size() << '\n';

    auto hull_module = kipod.GetModule("Meshmodels");
    auto meshscene = static_cast<MeshModelModule*>(hull_module)->GetScene();
    auto hullmesh = MeshModel(v, i);
    meshscene->AddModel(hullmesh);

    kipod.Run();
    kipod.ShutDown();

    return 0;
}
