#include "quasicrystals_scene.h"
#include "quasicrystals_physical_window.h"

namespace quacry{

void QuasiCrystalsScene::ProcessKeys(KeyPressedEvent &event)
{

}

void QuasiCrystalsScene::ProcessMouseButtons(MouseButtonEvent &event)
{

}

void QuasiCrystalsScene::ProcessMouseMoves(MouseMoveEvent &event)
{

}

void QuasiCrystalsScene::SetupLayout(QuasiCrystal *quacry)
{

}

void QuasiCrystalsScene::SetupShaders()
{
    shaders_.insert({"Quasi Physical", std::make_shared<kipod::Shader>("shaders/inside_polygon.vert.glsl", "shaders/points.frag.glsl")});
    shaders_.insert({"Quasi Internal", std::make_shared<kipod::Shader>("shaders/inside_polygon_window.vert.glsl", "shaders/points.frag.glsl")});

    shaders_.insert({"Quasi Physical with Edges", std::make_shared<kipod::Shader>("shaders/inside_polygon_with_edges.vert.glsl", "shaders/points.frag.glsl", "shaders/edges.geom.glsl")});

    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical with Edges"]->AttachUniform<int>("n");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("basis");

    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical"]->AttachUniform<int>("n");
    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("basis");
    shaders_["Quasi Physical"]->AttachUniform<float>("point_size");
    shaders_["Quasi Physical"]->AttachUniform<float>("alpha");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("color_z");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("color_w");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("bound_zw");

    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("shape_transform");
    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Internal"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Internal"]->AttachUniform<float>("point_size");
    shaders_["Quasi Internal"]->AttachUniform<int>("n");

    shaders_.insert({"Shape", std::make_shared<kipod::Shader>("kipod/shaders/shape.vert.glsl",   "kipod/shaders/shape.frag.glsl")});
    shaders_["Shape"]->AttachUniform<float>("depth");
    shaders_["Shape"]->AttachUniform<glm::mat4>("transform");
    shaders_["Shape"]->AttachUniform<glm::vec4>("color");
}

void QuasiCrystalsScene::SetupUniforms(Projection *projection, QuasiCrystal *quacry)
{
    SetUniformInternal(projection, quacry);
    SetUniformPhysical(projection, quacry);
    SetUniformPhysicalBox(projection, quacry);
}

void QuasiCrystalsScene::SetUniformInternal(Projection *projection, QuasiCrystal *quacry)
{
    auto data = quacry->lattice_data_.get();
    auto window = quacry->window_.get();

    auto shader = shaders_["Quasi Internal"];
    shader->SetUniform<glm::mat4>("shape_transform", window->TransformWorld());

    shader->SetUniform<glm::mat4>("pv",  *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->Transform());

    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<float>("point_size", data->point_size_window_);
}

void QuasiCrystalsScene::SetUniformPhysical(Projection *projection, QuasiCrystal *quacry)
{
    auto data = quacry->lattice_data_.get();
    auto window = quacry->window_.get();

    auto shader = shaders_["Quasi Physical"];

    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->Transform());
    shader->SetUniform<glm::mat4>("basis", quacry->GetBasis());
    shader->SetUniform<float>("point_size", data->point_size_);
    shader->SetUniform<float>("alpha", data->alpha_);
    shader->SetUniform<glm::vec4>("color_z", data->color_z_);
    shader->SetUniform<glm::vec4>("color_w", data->color_w_);
    glm::vec4 bound;
    auto z_mima = std::minmax_element(window->transformed_vertices_.begin(), window->transformed_vertices_.end(), [](vec2 v, vec2 w){ return v.x < w.x; });
    auto w_mima = std::minmax_element(window->transformed_vertices_.begin(), window->transformed_vertices_.end(), [](vec2 v, vec2 w){ return v.y < w.y; });
    bound.x = -(*z_mima.first).x;
    bound.y = -(*w_mima.first).y;
    bound.z = (*z_mima.second).x + bound.x;
    bound.w = (*w_mima.second).y + bound.y;
    shader->SetUniform<glm::vec4>("bound_zw", bound);
}

void QuasiCrystalsScene::SetUniformPhysicalWithEdges(Projection *projection, QuasiCrystal *quacry)
{
    auto data = quacry->lattice_data_.get();
    auto window = quacry->window_.get();

    auto shader = shaders_["Quasi Physical with Edges"];

    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->Transform());
    shader->SetUniform<glm::mat4>("basis", quacry->GetBasis());
}

void QuasiCrystalsScene::SetUniformPhysicalBox(Projection *projection, QuasiCrystal *quacry)
{
//    auto data = quacry->lattice_data_;
//    auto window_size = quacry->window_size_;

//    auto shader = shaders_["Quasi Physical Box"];

//    shader->SetUniform<glm::mat4>("pv", *camera);
//    shader->SetUniform<glm::mat4>("transform", quacry->PointSet::GetWorldTransform());


//    glBindBuffer(GL_UNIFORM_BUFFER, data->u_buffer);
//    glBufferData(GL_UNIFORM_BUFFER, size(window_size)*sizeof(float), window_size.data(), GL_DYNAMIC_DRAW);
//    glBindBufferBase(GL_UNIFORM_BUFFER, data->u_binding_point, data->u_buffer);
}

QuasiCrystalsScene::QuasiCrystalsScene(int width, int height) : RenderScene(width, height)
{
    framebuffers_["Physical Space"] = framebuffers_["Module Viewport"];
    framebuffers_.erase("Module Viewport");
    framebuffers_["Internal Space"] = std::make_shared<kipod::FrameBuffer>(width_, height_);
}

void QuasiCrystalsScene::Signup()
{

}

void QuasiCrystalsScene::Receive(std::shared_ptr<Event> event)
{

}

void QuasiCrystalsScene::Setup()
{
    name_ = "Quasicrystals";
    SetupShaders();

    PhysicalWindow pw;
    projections_.emplace_back(std::make_unique<Projection>(
                                  pw.Left(),
                                  pw.Right(),
                                  pw.Bottom(),
                                  pw.Top(),
                                  pw.Near(),
                                  pw.Far()));
    ActiveProjection(projections_.back().get());

    glEnable(GL_PROGRAM_POINT_SIZE);
    LOG_INFO("Quasicrystals scene initialized.");
    LOG_CONSOLE("Quasicrystals scene initialized.");
}

void QuasiCrystalsScene::Draw()
{
    if(auto quacry = ActiveQuasiCrystal()){
        framebuffers_["Physical Space"]->Bind();
        glEnable( GL_BLEND );
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaders_["Quasi Physical"]->Use();
        SetUniformPhysical(ActiveProjection(), quacry);
        quacry->Draw();
        if( quacry->lattice_data_->edges_ ){
            shaders_["Quasi Physical with Edges"]->Use();
            SetUniformPhysicalWithEdges(ActiveProjection(), quacry);
            quacry->Draw();
        }

        framebuffers_["Internal Space"]->Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shaders_["Shape"]->Use();
        SetUniformWindow(quacry->window_.get());
        quacry->window_->Draw();

        shaders_["Quasi Internal"]->Use();
        SetUniformInternal(ActiveProjection(), quacry);
        quacry->Draw();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
    kipod::RenderManager::Bind(0);
}

void QuasiCrystalsScene::AddQuasiCrystal(QuasiCrystal &&quacry)
{
    quacries_.emplace_back(
                std::make_unique<QuasiCrystal>(
                    std::move(quacry)));
}

QuasiCrystal *QuasiCrystalsScene::ActiveQuasiCrystal()
{
    return active_quacry_;
}

void QuasiCrystalsScene::ActiveQuasiCrystal(QuasiCrystal * active)
{
    active_quacry_ = active;
}

bool QuasiCrystalsScene::HasQuasiCrystal()
{
    return !quacries_.empty();
}

QuasiCrystalsScene::Projection *QuasiCrystalsScene::ActiveProjection()
{
    return GetActiveCamera();
}

void QuasiCrystalsScene::ActiveProjection(QuasiCrystalsScene::Projection* projection)
{
    active_camera_= projection;
}

void QuasiCrystalsScene::SetUniformWindow(Window* window)
{
    shaders_["Shape"]->SetUniform<glm::mat4>("transform", window->TransformWorld());
}
}
