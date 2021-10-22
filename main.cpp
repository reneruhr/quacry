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

    int width = 600, height = 400;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(QuasiCrystalsModule("Quasicrystals", width, height));

    auto quacry = kipod.GetModule("Quasicrystals");
    auto scene = static_cast<QuasiCrystalsModule*>(quacry)->GetScene();
    auto menu = static_cast<QuasiCrystalsModule*>(quacry)->GetScene();
    
    scene->SetFixedRatio(false);
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
