#include "quasicrystals_scene.h"
#include "quasicrystals_physical_window.h"

namespace kipod::QuasiCrystals{

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
    //shaders_.insert({"Quasi Physical Box", std::make_shared<kipod::Shader>("shaders/points.vert.glsl", "shaders/points.frag.glsl")});
    shaders_.insert({"Quasi Internal", std::make_shared<kipod::Shader>("shaders/inside_polygon_window.vert.glsl", "shaders/points.frag.glsl")});
    //shaders_.insert({"Quasi Physical Texture", std::make_shared<kipod::Shader>("shaders/inside_polygon.vert.glsl", "kipod/shaders/renderToTexture.frag.glsl")});

    //shaders_.insert({"PointSet", std::make_shared<kipod::Shader>("shaders/points.vert.glsl", "shaders/points.frag.glsl")});

    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical"]->AttachUniform<float>("point_size");
    shaders_["Quasi Physical"]->AttachUniform<float>("alpha");
    shaders_["Quasi Physical"]->AttachUniform<float>("zdecay");
    shaders_["Quasi Physical"]->AttachUniform<float>("wdecay");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("zColor");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("wColor");
    shaders_["Quasi Physical"]->AttachUniform<int>("n");

    shaders_["Quasi Internal"]->AttachUniform<float>("depth");
    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("shape_transform");
    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Internal"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Internal"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Internal"]->AttachUniform<float>("point_size");
    shaders_["Quasi Internal"]->AttachUniform<float>("alpha");
    shaders_["Quasi Internal"]->AttachUniform<float>("zdecay");
    shaders_["Quasi Internal"]->AttachUniform<float>("wdecay");
    shaders_["Quasi Internal"]->AttachUniform<glm::vec4>("zColor");
    shaders_["Quasi Internal"]->AttachUniform<glm::vec4>("wColor");
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

    vec4 colorw = {0.3f,0.3f,0.3f,1.0f}; // data.w_color_;
    auto colorw_glm = MakeGLM(colorw);
    vec4 colorz =  {0.3f,0.3f,0.3f,1.0f}; //data.z_color_;
    auto colorz_glm = MakeGLM(colorz);

    auto shader = shaders_["Quasi Internal"];
    shader->SetUniform<float>("depth", data->depth_);
    shader->SetUniform<glm::mat4>("shape_transform", window->TransformWorld());

    shader->SetUniform<glm::mat4>("pv",  *projection);
    shader->SetUniform<glm::mat4>("transform", quacry->TransformWorld());
    //std::vector<glm::vec2> shape_vectors = vec2vecToGLM(quacry->window_->transformed_vertices_);
    //shader->SetUniform< std::vector< glm::vec2 > >("shape", shape_vectors, 8);
    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, window->NumberEdges(), &window->transformed_vertices_[0][0]);

    shader->SetUniform<int>("n", window->NumberEdges());
    shader->SetUniform<float>("point_size", data->point_size_window_);
    shader->SetUniform<float>("alpha", 1.0f);
    shader->SetUniform<float>("zdecay", data->z_decay_);
    shader->SetUniform<float>("wdecay", data->w_decay_);
    shader->SetUniform<glm::vec4>("zColor", colorz_glm);
    shader->SetUniform<glm::vec4>("wColor", colorw_glm);
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
    shader->SetUniform<glm::mat4>("transform", quacry->TransformWorld());
    shader->SetUniform<float>("point_size", data->point_size_);
    shader->SetUniform<float>("alpha", data->alpha_);
    shader->SetUniform<float>("zdecay", data->z_decay_);
    shader->SetUniform<float>("wdecay", data->w_decay_);
    shader->SetUniform<glm::vec4>("zColor", data->z_color_);
    shader->SetUniform<glm::vec4>("wColor", data->w_color_);
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
    name_ = "Quasi Crystals";
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
    LOG_INFO("Quasi Crystals Scene Initialized.");
    LOG_CONSOLE("Quasi Crystals Scene Initialized.");
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
    shaders_["Shape"]->SetUniform<float>("depth", window->depth_);
}
}
