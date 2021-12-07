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
    if (auto quacry = dynamic_cast<Quasicrystal22 *>(scene->ActiveQuasiCrystal())) {

        static glm::mat4 current_transform = glm::mat4(1.0);

        static SL2Embedding current_embedding = NOT_DEFINED;
        static vector<SL2InSL4> SL2stack;
        static MatrixWalk SL4walk;
        static std::array<int, 16> selectedEmbedding{1, 1, 0, 0,
                                                     1, 1, 0, 0,
                                                     0, 0, 0, 0,
                                                     0, 0, 0, 0};
        SL2Control(current_transform, SL4walk, current_embedding);
        ImGui::Columns(3, NULL, true);
        DrawEmbeddings(selectedEmbedding, current_embedding);
        ImGui::NextColumn();
        EmbeddingsView(selectedEmbedding);
        ImGui::Columns(1);
        quacry->world_->Replace(current_transform);

        if (ImGui::TreeNode("Walk")) {
            MatrixWalkCandidate(SL4walk);
            SL4Control(SL4walk);
            ImGui::TreePop();
        } // Walk
    } else if (auto quacry = dynamic_cast<Quasicrystal23 *>(scene->ActiveQuasiCrystal())) {

        static glm::mat4 current_transform23 = glm::mat4(1.0);

        static SL2Embedding current_embedding23 = NOT_DEFINED;
        static vector<SL2InSL4> SL2stack23;
        static MatrixWalk SL4walk23;
        static std::array<int, 16> selectedEmbedding23{1, 1, 0, 0,
                                                       1, 1, 0, 0,
                                                       0, 0, 0, 0,
                                                       0, 0, 0, 0};
        bool changed  = SL2Control(current_transform23, SL4walk23, current_embedding23);
        ImGui::Columns(3, NULL, true);
        DrawEmbeddings(selectedEmbedding23, current_embedding23);
        ImGui::NextColumn();
        EmbeddingsView5(selectedEmbedding23);
        ImGui::Columns(1);

        if(changed) {
            auto new_lattice = Mat4ToEigen5(current_transform23);
            quacry->MultiplyLatticeAndLLL(new_lattice);
            scene->UpdateQuasicrystal();
        }

        if (ImGui::TreeNode("Walk#5")) {
            MatrixWalkCandidate(SL4walk23);
            SL4Control(SL4walk23);
            ImGui::TreePop();
        }
    }
}
}