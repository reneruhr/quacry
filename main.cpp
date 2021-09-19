#include "kipod.h"

#include "kipod/src/modules/meshmodels/meshmodel_module.h"
#include "src/quasicrystals_module.h"
//#include "src/test/convex_hull_test.h"

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
    kipod.Add(quacry::QuasiCrystalsModule("Quasi-Crystals", width, height));
    kipod.Add(kipod::MeshModels::MeshModelModule("Meshmodels", width,height));

//    ConvexHullTest(kipod); 
     
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
