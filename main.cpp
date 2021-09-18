#include "kipod.h"

#include "kipod/src/modules/meshmodels/meshmodel_module.h"
#include "src/quasicrystals_module.h"
#include "external_libs/quickhull/QuickHull.hpp"

extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;

int main( int argc, char **argv )
{
//    int width = 1024, height = 768;
    //int width = 512, height = 384;

    int width = 800, height = 800;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(quacry::QuasiCrystalsModule("Quasi-Crystals", width, height));
    kipod.Add(kipod::MeshModels::MeshModelModule("Meshmodels", width,height));
    kipod.Add(kipod::MeshModels::MeshModelModule("Convex Hull Test", width,height));

    using namespace quickhull;
    QuickHull<float> qh;
    /*
  
    auto hull_module = kipod.Module("Convex Hull Test");
    auto scene = hull_module->Scene();

    auto bob = scene->AddModel("kipod/resources/bob.obj");
     
    auto vertices = bob->Vertices();

    auto hull = qh.getConvexHull(vertices.data(), size(vertices), true, false);

    auto bobs_hull = MeshModel(hull.getIndexBuffer(), hull.getVertexBuffer();

    scene->AddModel(bobs_hull);
     
     
     */
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
