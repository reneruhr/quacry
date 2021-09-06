#include "quasicrystals_module.h"
#include "quasicrystals_sidebar.h"
#include "quasicrystals_scene.h"
#include "quasicrystals_console.h"
namespace quacry{

QuasiCrystalsModule::QuasiCrystalsModule(std::string name, int width, int height) : Module(name)
{
    LOG_INFO("Meshmodel OpenGL Render Module constructed");
    scene_ = std::make_shared<QuasiCrystalsScene>(width, height);
    sidebar_ = std::make_unique<QuasiCrystalsSidebar>(QuasiCrystalsSidebar(scene_));
    console_ = std::make_unique<QuasiCrystalsConsole>(QuasiCrystalsConsole(scene_));
}
}
