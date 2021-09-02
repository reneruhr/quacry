#include "kipod.h"

#include "quasicrystals_module.h"

extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;

int main( int argc, char **argv )
{

//    int width = 1024, height = 768;
    int width = 512, height = 384;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(kipod::QuasiCrystals::QuasiCrystalsModule("Quasi-Crystals", width,height));
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
