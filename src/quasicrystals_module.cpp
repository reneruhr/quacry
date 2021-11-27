#include "quasicrystals_module.h"
#include "quasicrystals_sidebar.h"
#include "quasicrystals_scene.h"
#include "quasicrystals_console.h"
#include "quasicrystals_examples.h"
#include <memory>
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
    if (ImGui::BeginMenu("Examples")) {
        if(ImGui::MenuItem("Ammann-Beenker" ,  "", false)){
            LOG_CONSOLE("Added Ammann Beenker");
            scene_->AddQuasiCrystal(AmmannBeenker());
            scene_->ActiveQuasiCrystal(scene_->quacries_.back().get());
        }
        else if(ImGui::MenuItem("Penrose",  "", false)){
            LOG_CONSOLE("Added Penrose");
            scene_->AddQuasiCrystal(Penrose());
            scene_->ActiveQuasiCrystal(scene_->quacries_.back().get());
        }
        else if(ImGui::MenuItem("Quasicrystal23 Cubic Window",  "", false)){
            LOG_CONSOLE("Added Cubic Quasicrystal");
            scene_->AddQuasiCrystal(MakeGenericQuasi23(2.2f));
            scene_->ActiveQuasiCrystal(scene_->quacries_.back().get());
        }
        ImGui::EndMenu();
    }
    if( ImGui::BeginMenu("Record")){
        static int frames_per_shot = 30;
        if(ImGui::MenuItem("Record" ,  "", scene_->Toggle("Take Screenshot"))){
            scene_->Toggle("Take Screenshot").Switch();
        }
        if(ImGui::SliderInt("Frames per Screenshot", &frames_per_shot, 1, 120)) {
            scene_->SetFramesPerScreenshot(frames_per_shot);
        }
        ImGui::EndMenu();
    }
    if( ImGui::BeginMenu("Window")){
        if(ImGui::MenuItem("Pause Window Updates" ,  "", scene_->Toggle("Pause Window Updates"))){
            scene_->Toggle("Pause Window Updates").Switch();
        }
        if(ImGui::MenuItem("Update now", "", false)){
            scene_->UpdateQuasicrystal(true);
        }
        ImGui::EndMenu();
    }
}
}

