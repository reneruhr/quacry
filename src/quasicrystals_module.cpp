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
    menu_= std::make_unique<QuasiCrystalsModuleMenu>(static_cast<QuasiCrystalsScene*>(scene_.get()));
}

auto QuasiCrystalsModule::GetScene() -> QuasiCrystalsScene*  
{ 
    return static_cast<QuasiCrystalsScene*>(scene_.get()); 
}

void QuasiCrystalsModuleMenu::Draw(){
    if (ImGui::BeginMenu("Examples"))
    {
        if(ImGui::MenuItem("Example 1" ,  "", true)){
        }
            ImGui::EndMenu();
        }
}
}

