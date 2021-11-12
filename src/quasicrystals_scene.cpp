#include "quasicrystals_scene.h"
#include "quasicrystals_physical_window.h"

namespace quacry {

void QuasiCrystalsScene::ProcessKeys(KeyPressedEvent &event)
{
    float stepsize = 1.0f;
    auto key = event.GetKeyCode();
    auto mod = event.GetMod();

    if (key == Key::Left && mod == Mod::None) {
        ActiveProjection()->Move(kipod::RenderCamera::Movement::LEFT, stepsize);
    } else if (key == Key::Right && mod == Mod::None) {
        ActiveProjection()->Move(kipod::RenderCamera::Movement::RIGHT, stepsize);
    } else if (key == Key::PageUp && mod == Mod::None) {
        ActiveProjection()->ScaleOrthogonalCamera(stepsize);
    } else if (key == Key::PageDown && mod == Mod::None) {
        ActiveProjection()->ScaleOrthogonalCamera(-stepsize);
    } else if (key == Key::Up && mod == Mod::None) {
        ActiveProjection()->Move(kipod::RenderCamera::Movement::UP, stepsize);
    } else if (key == Key::Down && mod == Mod::None) {
        ActiveProjection()->Move(kipod::RenderCamera::Movement::DOWN, stepsize);
    }
}

void QuasiCrystalsScene::ProcessMouseButtons(MouseButtonEvent &event)
{

}

void QuasiCrystalsScene::ProcessMouseMoves(MouseMoveEvent &event)
{

}

void QuasiCrystalsScene::SetupOptions()
{
    Add(kipod::ModeToggle("Take Screenshot", false));
    Add(kipod::ModeToggle("Draw Rejected", false));
    Add(kipod::ModeToggle("Pause Window Updates", false));
}

void QuasiCrystalsScene::SetupMeshModelModule()
{
    auto module = MeshModelModule("MeshModels", width_, height_);
    module.Init();
    meshmodel_module_ = std::make_unique<MeshModelModule>(std::move(module));
    GetMeshModelScene()->Toggle("Prepare Screen").Off();
    GetMeshModelScene()->Signup();
    GetMeshModelScene()->Add("Left", Key::J);
    GetMeshModelScene()->Add("Right", Key::L);
    GetMeshModelScene()->Add("Up", Key::I);
    GetMeshModelScene()->Add("Forward", Key::P);
    GetMeshModelScene()->Add("Down", Key::K);
    GetMeshModelScene()->Add("Backward", Key::Semicolon);
}

auto QuasiCrystalsScene::GetMeshModelScene() -> MeshModelScene *
{
    return meshmodel_module_->GetScene();
}

auto QuasiCrystalsScene::GetMeshModelModule() -> MeshModelModule *
{
    return meshmodel_module_.get();
}

void QuasiCrystalsScene::SetupShaders()
{
    shaders_.insert({"Quasi Physical",
                     std::make_shared<kipod::Shader>("shaders/points.vert.glsl", "shaders/points.frag.glsl",
                                                     "shaders/points.geom.glsl")});
    shaders_.insert({"Quasi Internal", std::make_shared<kipod::Shader>("shaders/inside_polygon_window.vert.glsl",
                                                                       "shaders/points.frag.glsl")});

    shaders_.insert({"Quasi Physical with Edges",
                     std::make_shared<kipod::Shader>("shaders/points.vert.glsl", "shaders/points.frag.glsl",
                                                     "shaders/edges.geom.glsl")});

    shaders_.insert({"Quasicrystal23 Points",
                     std::make_shared<kipod::Shader>("shaders/points23.vert.glsl", "shaders/points.frag.glsl",
                                                     "shaders/points23.geom.glsl")});

    shaders_["Quasicrystal23 Points"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasicrystal23 Points"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasicrystal23 Points"]->AttachUniform<glm::vec4>("color");
    shaders_["Quasicrystal23 Points"]->AttachUniform<int>("Space");
    shaders_["Quasicrystal23 Points"]->AttachUniform<float>("point_size");

    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical with Edges"]->AttachUniform<int>("n");
    shaders_["Quasi Physical with Edges"]->AttachUniform<glm::mat4>("basis");

    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical"]->AttachUniform<int>("n");
    shaders_["Quasi Physical"]->AttachUniform<int>("n_pattern");
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

    shaders_.insert({"Shape", std::make_shared<kipod::Shader>("shaders/shape.vert.glsl", "shaders/shape.frag.glsl")});
    shaders_["Shape"]->AttachUniform<float>("depth");
    shaders_["Shape"]->AttachUniform<glm::mat4>("transform");
    shaders_["Shape"]->AttachUniform<glm::mat4>("pv");
    shaders_["Shape"]->AttachUniform<glm::vec4>("color");

    shaders_.insert(
            {"Shape Pattern", std::make_shared<kipod::Shader>("shaders/shape.vert.glsl", "shaders/shape.frag.glsl")});
    shaders_["Shape Pattern"]->AttachUniform<float>("depth");
    shaders_["Shape Pattern"]->AttachUniform<glm::mat4>("transform");
    shaders_["Shape Pattern"]->AttachUniform<glm::mat4>("pv");
    shaders_["Shape Pattern"]->AttachUniform<glm::vec4>("color");
}

void QuasiCrystalsScene::SetupUniforms(Projection *projection, Quasicrystal22 *quacry)
{
    SetUniformInternal(projection, quacry);
    SetUniformPhysical(projection, quacry);
    SetUniformPhysicalBox(projection, quacry);
}

void QuasiCrystalsScene::SetUniformInternal(Projection *projection, Quasicrystal22 *quacry)
{
    auto data = quacry->view_data_.get();
    auto window = quacry->window_.get();

    auto shader = shaders_["Quasi Internal"];
    shader->SetUniform<glm::mat4>("shape_transform", window->TransformWorld());

    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->g_);

    GLint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<float>("point_size", data->point_size_window_);
}

void QuasiCrystalsScene::SetUniformPhysical(Projection *projection, Quasicrystal22 *quacry)
{
    auto data = quacry->view_data_.get();
    auto window = quacry->window_.get();
    auto pattern = quacry->active_pattern_;

    auto shader = shaders_["Quasi Physical"];

    GLint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberVertices(), &window->transformed_vertices_[0][0]);

    if (pattern) {
        shader->SetUniform<int>("n_pattern", pattern->NumberVertices());
        GLint pattern_id = glGetUniformLocation(*shader, "pattern");
        glUniform2fv(pattern_id, pattern->NumberVertices(), &pattern->transformed_vertices_[0][0]);
    } else
        shader->SetUniform<int>("n_pattern", 0);

    shader->SetUniform<int>("n", window->NumberVertices());
    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->g_);
    shader->SetUniform<glm::mat4>("basis", quacry->GetBasis());
    shader->SetUniform<float>("point_size", data->point_size_);
    shader->SetUniform<float>("alpha", data->alpha_);
    shader->SetUniform<glm::vec4>("color_z", data->color_z_);
    shader->SetUniform<glm::vec4>("color_w", data->color_w_);
    glm::vec4 bound;
    auto z_mima = std::minmax_element(window->transformed_vertices_.begin(), window->transformed_vertices_.end(),
                                      [](auto v, auto w) { return v.x < w.x; });
    auto w_mima = std::minmax_element(window->transformed_vertices_.begin(), window->transformed_vertices_.end(),
                                      [](auto v, auto w) { return v.y < w.y; });
    bound.x = -(*z_mima.first).x;
    bound.y = -(*w_mima.first).y;
    bound.z = (*z_mima.second).x + bound.x;
    bound.w = (*w_mima.second).y + bound.y;
    shader->SetUniform<glm::vec4>("bound_zw", bound);
}

void QuasiCrystalsScene::SetUniformPhysicalWithEdges(Projection *projection, Quasicrystal22 *quacry)
{
    auto data = quacry->view_data_.get();
    auto window = quacry->window_.get();

    auto shader = shaders_["Quasi Physical with Edges"];

    GLint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->g_);
    shader->SetUniform<glm::mat4>("basis", quacry->GetBasis());
}

void QuasiCrystalsScene::SetUniformQuasicrystal23(Projection *projection, Quasicrystal23 *quacry, Space space)
{
    auto data = quacry->view_data_.get();
    auto shader = shaders_["Quasicrystal23 Points"];
    shader->SetUniform<glm::mat4>("pv", *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->Transform());
    shader->SetUniform<glm::vec4>("color", data->color_);
    shader->SetUniform<int>("Space", static_cast<int>(space));
    shader->SetUniform<float>("point_size", data->point_size_);
}

void QuasiCrystalsScene::SetUniformPhysicalBox(Projection *projection, Quasicrystal22 *quacry)
{
//    auto data = quacry->view_data_;
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
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard);
    LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
}

void QuasiCrystalsScene::Receive(std::shared_ptr<Event> event)
{
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(QuasiCrystalsScene::ProcessKeys));
}

void QuasiCrystalsScene::Setup()
{
    name_ = "Quasicrystals";
    SetupShaders();

    SetupMeshModelModule();
    SetupOptions();

    PhysicalWindow pw;
    projections_.emplace_back(
            std::make_unique<Projection>(pw.Left(), pw.Right(), pw.Bottom(), pw.Top(), pw.Near(), pw.Far()));
    ActiveProjection(projections_.back().get());

    PhysicalWindow int_wind{{-3, 3, -3, 3, -5, 5, 0, 0}};
    internal_projection_ =
            std::make_unique<Projection>(int_wind.Left(), int_wind.Right(), int_wind.Bottom(), int_wind.Top(),
                                         int_wind.Near(), int_wind.Far());

    glEnable(GL_PROGRAM_POINT_SIZE);
    LOG_INFO("Quasicrystals scene initialized.");
    LOG_CONSOLE("Quasicrystals scene initialized.");
}

void QuasiCrystalsScene::Draw()
{
    frame_count_++;
    framebuffers_["Physical Space"]->Bind();
    glEnable(GL_BLEND);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (auto quacry = dynamic_cast<Quasicrystal22 *>(ActiveQuasiCrystal())) {
        quacry->ApplyLLL();

        shaders_["Quasi Physical"]->Use();
        SetUniformPhysical(ActiveProjection(), quacry);
        quacry->Draw();
        if (quacry->view_data_->edges_) {
            shaders_["Quasi Physical with Edges"]->Use();
            SetUniformPhysicalWithEdges(ActiveProjection(), quacry);
            quacry->Draw();
        }
        if (Toggle("Take Screenshot")) TakeScreenshot("Physical", false);

        framebuffers_["Internal Space"]->Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shaders_["Quasi Internal"]->Use();
        SetUniformInternal(internal_projection_.get(), quacry);
        quacry->Draw();

        if (quacry->view_data_->window_shape_) {
            shaders_["Shape"]->Use();
            SetUniformWindow(internal_projection_.get(), quacry);
            quacry->window_->Draw();
        }
        if (quacry->view_data_->pattern_shape_ && quacry->active_pattern_) {
            shaders_["Shape Pattern"]->Use();
            SetUniformPattern(internal_projection_.get(), quacry);
            quacry->active_pattern_->Draw();
        }

        if (Toggle("Take Screenshot")) TakeScreenshot("Internal", true);

    } else if (auto quacry = dynamic_cast<Quasicrystal23 *>(ActiveQuasiCrystal())) {
        shaders_["Quasicrystal23 Points"]->Use();
        SetUniformQuasicrystal23(ActiveProjection(), quacry, Space::Physical);
        quacry->Draw();
        if (Toggle("Take Screenshot")) TakeScreenshot("Physical", false);

        framebuffers_["Internal Space"]->Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shaders_["Quasicrystal23 Points"]->Use();
        SetUniformQuasicrystal23(GetMeshModelScene()->GetActiveCamera(), quacry, Space::Internal);
        quacry->Draw(Space::Internal);
        if (Toggle("Draw Rejected")) quacry->Draw(Space::Rejected);

        GetMeshModelScene()->Draw();

        if (Toggle("Take Screenshot")) TakeScreenshot("Internal", true);
    }
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    kipod::RenderManager::Bind(0);
}

void QuasiCrystalsScene::AddQuasiCrystal(Quasicrystal22 &&quacry)
{
    quacries_.emplace_back( std::make_unique<Quasicrystal22>( std::move(quacry)));
}

void QuasiCrystalsScene::AddQuasiCrystal(Quasicrystal23 &&quacry)
{
    quacries_.emplace_back( std::make_unique<Quasicrystal23>( std::move(quacry)));
    auto cur_quacry = static_cast<Quasicrystal23 *>(quacries_.back().get());

    auto window = cur_quacry->GetWindow();
    window->world_->move_sink_.connect<&QuasiCrystalsScene::UpdateQuasicrystal>(*this);
    window->local_->scale_sink_.connect<&QuasiCrystalsScene::UpdateQuasicrystal>(*this);

    cur_quacry->UpdateWindowPointer( GetMeshModelScene()->AddModel(cur_quacry->GiveUpWindow()) );

}

auto QuasiCrystalsScene::ActiveQuasiCrystal() -> Quasicrystal *
{
    return active_quacry_;
}

void QuasiCrystalsScene::ActiveQuasiCrystal(Quasicrystal *active)
{
    active_quacry_ = active;
    if (auto quasi23 = dynamic_cast<Quasicrystal23 *>(active))
        GetMeshModelScene()->SetActiveModel(quasi23->GetWindow());
}

bool QuasiCrystalsScene::HasQuasiCrystal()
{
    return !quacries_.empty();
}

QuasiCrystalsScene::Projection *QuasiCrystalsScene::ActiveProjection()
{
    return GetActiveCamera();
}

void QuasiCrystalsScene::ActiveProjection(QuasiCrystalsScene::Projection *projection)
{
    active_camera_ = projection;
}

void QuasiCrystalsScene::SetUniformWindow(Projection *projection, Quasicrystal22 *quacry)
{
    auto window = quacry->window_.get();
    shaders_["Shape"]->SetUniform<glm::mat4>("transform", window->TransformWorld());
    shaders_["Shape"]->SetUniform<glm::mat4>("pv", *projection);
    shaders_["Shape"]->SetUniform<glm::vec4>("color", quacry->view_data_->color_window_);
    shaders_["Shape"]->SetUniform<float>("depth", window->depth_);
}

void QuasiCrystalsScene::SetUniformPattern(Projection *projection, Quasicrystal22 *quacry)
{
    auto pattern = quacry->active_pattern_;
    auto window = quacry->window_.get();
    shaders_["Shape Pattern"]->SetUniform<glm::mat4>("transform", window->TransformWorld());
    shaders_["Shape Pattern"]->SetUniform<glm::mat4>("pv", *projection);
    shaders_["Shape Pattern"]->SetUniform<glm::vec4>("color", quacry->view_data_->color_pattern_);
    shaders_["Shape Pattern"]->SetUniform<float>("depth", pattern->depth_ + 0.1);
}

void QuasiCrystalsScene::UpdateQuasicrystal(bool force)
{
    LOG_CONSOLE("Called UpdateQuasicrystal function");
    if (auto quacry = dynamic_cast<Quasicrystal23 *>(ActiveQuasiCrystal()) ) {
        if(force || !Toggle("Pause Window Updates"))
            quacry->Resample();
    }
}
}