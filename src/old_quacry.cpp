#include "quacry.h"
//#include "scene.h"
#include "utils/mat.h"
#include "pointset.h"

#include "math/minkowski_embedding.h"
#include "gui.h"



QuaCry::QuaCry(Scene* scene, mat4 basis,
               std::vector< float > window_size,
               std::vector< int > sample_size,
               WindowType type,
               Shape shape)
        : PointSet(sample_size, basis),
          WindowBox(window_size),
          GUIModule(),
          GUIMathControl(),
          Shape(shape),
          scene_(scene),
          window_type_(type)
{
    Init();
}


QuaCry::QuaCry(Scene *scene) : PointSet(), WindowBox(), GUIModule(), GUIMathControl(), Shape(kipod::Shapes::Square()), scene_(scene)
{
    Init();
}

void QuaCry::Init()
{
    PointSet::Init();
    lattice_data_->qc.window = window_type_;
    lattice_data_->qc.shape = this;

    scene_->AddPointSet(this);

    camera_ = new Camera( Left(), Right(),
                          Bottom(), Top(),
                          Near(), Far() );
    //sideViewCamera_ = new Camera( -10, -10, -10, 10, -10, 10);
    sideViewCamera_ = new Camera(45, float(scene_->width_)/scene_->height_, 0.1f, 200.0);
    sideViewCamera_->updateLookAt(vec3(30,0,0),vec3(60,0,0),vec3(0,0,1));

    scene_->addCamera(sideViewCamera_);
    scene_->addCamera(camera_, false);
    scene_->setLastCameraActive();

    Shape::Init();
    scene_->AddShape(this);

    LOG("Inititalized QuaCry");
}

void QuaCry::Draw()
{
    if (ImGui::CollapsingHeader("Quasicrystals")){


    if (ImGui::TreeNode("Lattice:")){
            static mat4 f_basis;
            f_basis = basis_;
            static mat4 f_m;
            f_m= PointSet::world_transform_;
            ImGui::Columns(2, NULL, true);
            ImGui::Text("Basis:");
            matrix4(f_basis);
            ImGui::NextColumn();
            ImGui::Text("Current Transformation:");
            matrix4(f_m);
            ImGui::Columns(1);
            ImGui::TreePop();
    }// Lattice

    static mat4 scale_matrix;
    static mat2 window_matrix;
    static std::array<int, 16> selectedEmbedding { 1, 1, 0, 0,
                                                       1, 1, 0, 0,
                                                       0, 0, 0, 0,
                                                      0, 0, 0, 0 };
   if (ImGui::TreeNode("Embedded SL2:")){
            ImGui::Columns(3, NULL, true);
            embeddings(selectedEmbedding, currEmbedding);
            ImGui::NextColumn();
            embeddingsView(selectedEmbedding);
            ImGui::Columns(1);
            sl2control(temporaryMatrixView, SL4walk, currEmbedding);
            ImGui::TreePop();
            PointSet::world_transform_ = temporaryMatrixView*scale_matrix;
        } // Embedding

        if (ImGui::TreeNode("Walk")){
           sl4control(SL4walk);
           ImGui::TreePop();
        } // Walk

        if (ImGui::TreeNode("View:")){
            static int selected_view = -1;
            for (int n = 0; n <  scene_->numberOfCameras(); n++){
                char buf[64];
                if(n==0) sprintf(buf, "Perspective Projection to XYZ (close up) ");
                else if(n==1) sprintf(buf, "Perspective Projection to XYZ (far away)");
                else if(n==2) sprintf(buf, "Cut and Project");

                if (ImGui::Selectable(buf, selected_view == n)){
                    selected_view = n;
                    scene_->setActiveCamera(selected_view);
                }
            }
            ImGui::TreePop();
        }// View


//        if (ImGui::TreeNode("Animations")){
//                 ImGui::Columns(3, NULL, true);
//                 embeddings(selectedEmbedding, currEmbedding);
//                 ImGui::NextColumn();
//                 embeddingsView(selectedEmbedding);
//                 ImGui::Columns(1);
//                 sl2control(temporaryMatrixView, SL4walk, currEmbedding);
//                 ImGui::TreePop();
//                 PointSet::world_transform_ = temporaryMatrixView*scale_matrix;
//                 ImGui::TreePop();
//             }// Animations


        if (ImGui::TreeNode("Modify Lattice and Window")){
                static float lattice_scale=1;
                ImGui::Text("Scale Lattice (uniformly xyzw)");
                if (ImGui::SliderFloat("##LatticeScale", &lattice_scale, 0.2, 5.0f)){
                   scale_matrix = mat4(lattice_scale);
                   PointSet::world_transform_ = temporaryMatrixView*scale_matrix;
                }
                static float window_scale=1;
                ImGui::Text("Scale Window (uniformly zw)");
                if (ImGui::SliderFloat("##window_scale", &window_scale, 0.2, 5.0f)){
                   window_matrix = mat2(window_scale);
                   Polygon::transform_ = window_matrix;
                   UpdatedTransformedVertices();
                   scene_->_glrenderer->UpdateShape(this->shape_data_, &this->transformed_vertices_);
                }

               if(ImGui::Button("Transpose Lattice"))
                   BaseChange(scene_->_glrenderer, transpose(basis_));
               if(ImGui::Button("Amman Benker Sublattice Lattice"))
                   BaseChange(scene_->_glrenderer,
                              transpose(MinkowskiEmbedding(2).Embedding(
                               {{1,0},{0,0}},
                               {{0,1},{0,0}},
                               {{0,0},{2,0}},
                               {{0,0},{0,1}})));
               if(ImGui::Button("Full sqrt(2) Lattice"))
                   BaseChange(scene_->_glrenderer,
                              transpose(MinkowskiEmbedding(2).Embedding(
                               {{1,0},{0,0}},
                               {{0,1},{0,0}},
                               {{0,0},{1,0}},
                               {{0,0},{0,1}})));

         ImGui::TreePop();
        } // Scale

        if (ImGui::TreeNode("Looks")){
                static float point_size = lattice_data_->point_size_;
                static float point_size_window = lattice_data_->point_size_window_;

                ImGui::Text("Point Size");
                if (ImGui::SliderFloat("##PointScale", &point_size, 1.0f, 10.0f)){
                   SetPointSize(point_size);
                }
                ImGui::Text("Point Size in Window");
                if (ImGui::SliderFloat("##PointScaleWindow", &point_size_window, 0.1f, 10.0f)){
                   lattice_data_->point_size_window_ = point_size_window;
                }

                static float depth_slider = lattice_data_->depth_;
                ImGui::Text("Depth ZW points");
                if (ImGui::SliderFloat("##depth", &depth_slider, -1.2f, 1.2f)){
                   lattice_data_->depth_ = depth_slider;
                }
                static float depth_shader_slider = shape_data_->depth_;
                ImGui::Text("Depth Window");
                if (ImGui::SliderFloat("##depth2", &depth_shader_slider, -1.2f, 1.2f)){
                   shape_data_->depth_ = depth_shader_slider;
                }

                static float visibility_outside_window=0.01f;
                ImGui::Text("Change Transparency of points OUTSIDE the window");
                if (ImGui::SliderFloat("##VisibilityScale", &visibility_outside_window, 0.0, 1.0f)){
                   SetOutsideVisibility(visibility_outside_window);
                }

                static float zdecay=lattice_data_->z_decay_;
                static float wdecay=lattice_data_->w_decay_;
                ImGui::Text("Color decay from z-origin");
                if (ImGui::SliderFloat("z##zdecay", &zdecay, 0.01, 1.0f)){
                   lattice_data_->z_decay_=zdecay;
                }
                if (ImGui::SliderFloat("w##wdecay", &wdecay, 0.01, 1.0f)){
                   lattice_data_->w_decay_=wdecay;
                }

                static ImVec4 zColor = ImVec4(lattice_data_->z_color_[0],lattice_data_->z_color_[1],lattice_data_->z_color_[2],lattice_data_->z_color_[3]);
                static ImVec4 wColor = ImVec4(lattice_data_->w_color_[0],lattice_data_->w_color_[1],lattice_data_->w_color_[2],lattice_data_->w_color_[3]);
                ImGui::Text("Give the z/w-coordinates a colour!");
                if (ImGui::ColorEdit4("z##zColor", (float*)&zColor, 0)){
                   SetColorZW((float*)&zColor, (float*)&wColor);
                }
                if (ImGui::ColorEdit4("w##wColor", (float*)&wColor, 0)){
                   SetColorZW((float*)&zColor, (float*)&wColor);
                }



         ImGui::TreePop();
        } // Scale


    } //Quasicrystals
}

// Draw



        //        if (ImGui::CollapsingHeader("Sample Size")){
        //            GUIMathControl.verify4i(sampleSize, currentMode);
        //            if(currentMode == NEW_SAMPLE){
        //                lattice.new_sample(sampleSize);
        //                glClearColor(0, 0, 0, 0);
        //                GLfloat vertices_position[lattice.sample_num*4];

        //                lattice.set_vertices_position(vertices_position, 1);
        //                render.initialize(vao, vertices_position, lattice.sample_num);

        //                currentMode = IDLE;
        //            }
        //        }


//        if (ImGui::CollapsingHeader("Window Options")){
//            ImGui::Text("Size");
//            ImGui::SliderFloat4("zw window", windowBox.data(), -10.0f, 10.0f);
//        }

