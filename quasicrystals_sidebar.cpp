#include "quasicrystals_sidebar.h"
#include "quasicrystals_scene.h"
#include "../../engine/engine_gui.h"
namespace kipod::QuasiCrystals{

void QuasiCrystalsSidebar::SideBarContent()
{
    AddQuasiCrystal();
    QuasiCrystalsList();
}

void QuasiCrystalsSidebar::AddQuasiCrystal()
{
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);

    ImGui::PushID("Add Quacry");
    if(ImGui::Button("Add")){
                            LOG_CONSOLE("Please enter Basis in Console!");
                        }
    ImGui::PopID();
    ImGui::Separator();
}

void QuasiCrystalsSidebar::QuasiCrystalsList()
{
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);
    auto& quacries = scene->quacries_;
    static int selected_quacry = 0;
    if(scene->HasQuasiCrystal()){
        auto current_quacry = std::begin(quacries);
        for (int n = 0; n <  (int)quacries.size(); n++, ++current_quacry)
        {
            char buf[32];
            sprintf(buf, "Quasicrystal %d", n);
            if (ImGui::Selectable(buf, selected_quacry == n)){
                selected_quacry = n;
                scene->ActiveQuasiCrystal(current_quacry->get());
                }
            ImGui::SameLine();
        }
        ImGui::Separator();
    }
}

}
