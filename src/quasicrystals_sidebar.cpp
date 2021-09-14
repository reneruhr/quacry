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
            sprintf(buf, "%s", quacries[n]->name_.c_str());
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
        DrawColumnMatrix4(quacry->GetBasis());
        ImGui::Text("Current Transformation:");
        DrawColumnMatrix4(quacry->Transform());

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
            ImGui::Text("Scale View on Window");
            if (ImGui::SliderFloat("##window_scale", &window_scale, 0.2, 5.0f)){
               window_matrix = glm::mat2(window_scale);
               window->world_->Replace(window_matrix);
               window->UpdatedTransformedVertices();
               window->UpdateShape();
            }
            static int selected_pattern = quacry->patterns_.size();
            for (auto b = std::begin(quacry->patterns_), 
                 c = std::begin(quacry->patterns_),
                 e = std::end(quacry->patterns_); b<=e; ++b)
            {
                int n = std::distance(c,b);
                char buf[32];
                if(b!=e) sprintf(buf, "Pattern %d", n);
                else sprintf(buf, "No Pattern");
                if (ImGui::Selectable(buf, selected_pattern == n)){
                    selected_pattern = n;
                    quacry->active_pattern_ = b==e ? nullptr : (*b).get();
                }
            }
    }
    }
}

void SetOutsideVisibility(ViewData* data, float alpha)
{
    data->alpha_ = alpha;
}

void SetColorZW(ViewData* data, float* z, float* w)
{
    data->color_z_ = glm::make_vec4(z);
    data->color_w_ = glm::make_vec4(w);
}

void SetPointSize(ViewData* data, float size)
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
        auto data = quacry->view_data_.get();

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

        if (ImGui::TreeNode("Projections:")){
            auto projection = scene->ActiveProjection();
            static int selected_view = 0;
            for (int n = 0; n <  scene->NumberOfCameras(); n++){
                char buf[64];
                sprintf(buf, "Projection %d", n);

                if (ImGui::Selectable(buf, selected_view == n)){
                    selected_view = n;
                    scene->ActiveProjection(scene->projections_[selected_view].get());
                }
            }
            ImGui::InputFloat2("##camerapos", &projection->Eye()[0], "%.1f");
            kipod::HoverInfo("Controls", "Keyboard controls only work if the 'Viewport' window is active. "
                             "Click inside to active it.\n"
                             "Move Projection with Arrow & Page Down/Up Keys.\n");
            ImGui::TreePop();
        }// View
    }
    }
}
}
