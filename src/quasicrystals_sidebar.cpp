#include "quasicrystals_sidebar.h"
#include "quasicrystals_scene.h"
#include "math/minkowski_embedding.h"
#include "quasicrystals_examples.h"
#include "quasicrystals_guimathcontrol.h"
namespace quacry{

void QuasiCrystalsSidebar::SideBarContent()
{
    AddQuasiCrystal();
    QuasiCrystalsList();
    LatticeControl();
    WindowControl();
    ViewOptions();
}

void QuasiCrystalsSidebar::AddQuasiCrystal()
{
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);

    ImGui::PushID("Add Quacry");
    if(ImGui::Button("Add")){
        LOG_CONSOLE("Please enter Basis in Console!");
    }
    ImGui::PopID();
    ImGui::PushID("Add Quacry Examples");
    if(ImGui::Button("Add Ammann Beenker")){
        LOG_CONSOLE("Added Ammann Beenker");
        scene->AddQuasiCrystal(AmmannBeenker());
        scene->ActiveQuasiCrystal(scene->quacries_.back().get());
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

void QuasiCrystalsSidebar::LatticeControl()
{
    if (ImGui::CollapsingHeader("Lattice")){

    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);
    auto quacry = scene->ActiveQuasiCrystal();

    static glm::mat4 scale_matrix = glm::mat4(1.0);
    static glm::mat4 current_transform = glm::mat4(1.0);

    if (quacry){ 
        if(ImGui::Button("Transpose Lattice"))
           quacry->BaseChange(transpose(quacry->GetBasis()));
        static float lattice_scale=1;
        ImGui::Text("Scale Lattice (uniformly xyzw)");
        if (ImGui::SliderFloat("##LatticeScale", &lattice_scale, 0.2, 5.0f)){
           scale_matrix = glm::mat4(lattice_scale);
           quacry->local_->Replace(scale_matrix);
        }
        ImGui::Text("Basis:");
        DrawColumnMatrix4(quacry->basis_);
        ImGui::Text("Current Transformation:");
        DrawColumnMatrix4(quacry->Transform());

        /*
        if (ImGui::TreeNode("View:")){
            static int selected_view = -1;
            for (int n = 0; n <  scene->numberOfCameras(); n++){
                char buf[64];
                if(n==0) sprintf(buf, "Perspective Projection to XYZ (close up) ");
                else if(n==1) sprintf(buf, "Perspective Projection to XYZ (far away)");
                else if(n==2) sprintf(buf, "Cut and Project");

                if (ImGui::Selectable(buf, selected_view == n)){
                    selected_view = n;
                    scene->setActiveCamera(selected_view);
                }
            }
            ImGui::TreePop();
        }// View
        */

    } // quacry
    } //Lattice control
}

void QuasiCrystalsSidebar::WindowControl()
{
    if (ImGui::CollapsingHeader("Window")){

    static glm::mat2 window_matrix;
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);
    auto quacry = scene->ActiveQuasiCrystal();

    if (quacry){ // ImGui::TreeNode("Modify Lattice and Window") &&
            auto window = quacry->window_.get();

            static float window_scale=1;
            ImGui::Text("Scale Window (uniformly zw)");
            if (ImGui::SliderFloat("##window_scale", &window_scale, 0.2, 5.0f)){
               window_matrix = glm::mat2(window_scale);
               window->world_->Replace(window_matrix);
               window->UpdatedTransformedVertices();
               window->UpdateShape();
            }

           if(ImGui::Button("Ammann–Beenker Sublattice"))
               quacry->BaseChange(transpose(MinkowskiEmbedding(2).Embedding(
                           {{1,0},{0,0}},
                           {{0,1},{0,0}},
                           {{0,0},{2,0}},
                           {{0,0},{0,1}})));
           if(ImGui::Button("Full sqrt(2) Lattice"))
               quacry->BaseChange(transpose(MinkowskiEmbedding(2).Embedding(
                           {{1,0},{0,0}},
                           {{0,1},{0,0}},
                           {{0,0},{1,0}},
                           {{0,0},{0,1}})));

    // ImGui::TreePop();
    }
    }
}

void SetOutsideVisibility(LatticeData* data, float alpha)
{
    data->alpha_ = alpha;
}

void SetColorZW(LatticeData* data, float* z, float* w)
{
    data->color_z_ = glm::make_vec4(z);
    data->color_w_ = glm::make_vec4(w);
}

void SetPointSize(LatticeData* data, float size)
{
    data->point_size_ = size;
}

void QuasiCrystalsSidebar::ViewOptions()
{
    if (ImGui::CollapsingHeader("View Options")){
    auto scene = std::static_pointer_cast<QuasiCrystalsScene>(scene_);
    auto quacry = scene->ActiveQuasiCrystal();

    if ( quacry){ // ImGui::TreeNode("Looks") &&
        auto window = quacry->window_.get();
        auto data = quacry->lattice_data_.get();

        static float point_size = data->point_size_;
        static float point_size_window = data->point_size_window_;

        static bool edges = false;
        if(ImGui::Checkbox("Show edges", &edges)){
            data->edges_ = edges;
        }

        ImGui::Text("Point Size");
        if (ImGui::SliderFloat("##PointScale", &point_size, 1.0f, 10.0f)){
           SetPointSize(data, point_size);
        }

        ImGui::Text("Point Size in Window");
        if (ImGui::SliderFloat("##PointScaleWindow", &point_size_window, 0.1f, 10.0f)){
           data->point_size_window_ = point_size_window;
        }

        static float visibility_outside_window=0.01f;
        ImGui::Text("Change Transparency of points OUTSIDE the window");
        if (ImGui::SliderFloat("##VisibilityScale", &visibility_outside_window, 0.0, 1.0f)){
           SetOutsideVisibility(data, visibility_outside_window);
        }
/*
        static ImVec4 zColor = ImVec4(data->color_z_[0],data->color_z_[1],data->color_z_[2],data->color_z_[3]);
        static ImVec4 wColor = ImVec4(data->color_w_[0],data->color_w_[1],data->color_w_[2],data->color_w_[3]);
        ImGui::Text("Give the z/w-coordinates a colour!");
        if (ImGui::ColorEdit4("z##zColor", (float*)&zColor, 0)){
           SetColorZW(data, (float*)&zColor, (float*)&wColor);
        }
        if (ImGui::ColorEdit4("w##wColor", (float*)&wColor, 0)){
           SetColorZW(data, (float*)&zColor, (float*)&wColor);
        }
        */
    }
    }
}
}
