#include "quasicrystals_console.h"
#include "quasicrystals_scene.h"
#include "quasicrystals_guimathcontrol.h"

namespace quacry{

void QuasiCrystalsConsole::ConsoleContent(){
    SL2Action();
}

void QuasiCrystalsConsole::SL2Action()
{
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);
    auto quacry = dynamic_cast<Quasicrystal22*>(scene->ActiveQuasiCrystal());

    static glm::mat4 current_transform = glm::mat4(1.0);

    if (quacry){ 
    static SL2Embedding current_embedding = NOT_DEFINED;
    static vector<SL2InSL4> SL2stack;
    static MatrixWalk SL4walk;
    static std::array<int, 16> selectedEmbedding { 1, 1, 0, 0,
                                                   1, 1, 0, 0,
                                                   0, 0, 0, 0,
                                                   0, 0, 0, 0 };
    SL2Control(current_transform, SL4walk, current_embedding);
    ImGui::Columns(3, NULL, true);
    DrawEmbeddings(selectedEmbedding, current_embedding);
    ImGui::NextColumn();
    EmbeddingsView(selectedEmbedding);
    ImGui::Columns(1);
    quacry->world_->Replace(current_transform);

    if (ImGui::TreeNode("Walk")){
        MatrixWalkCandidate(SL4walk);
        SL4Control(SL4walk);
        ImGui::TreePop();
    } // Walk
    } // quacry
}
}
