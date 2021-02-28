#include "quasicrystals_scene.h"

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
    shaders_.insert({"Quasi Physical", std::make_shared<kipod::Shader>("inside_polygon.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Physical Box", std::make_shared<kipod::Shader>("points.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Internal", std::make_shared<kipod::Shader>("inside_polygon_window.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Physical Texture", std::make_shared<kipod::Shader>("inside_polygon.vert.glsl", "renderToTexture.frag.glsl")});

    shaders_.insert({"PointSet", std::make_shared<kipod::Shader>("points.vert.glsl", "points.frag.glsl")});

    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical"]->AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical"]->AttachUniform<float>("point_size");
    shaders_["Quasi Physical"]->AttachUniform<float>("alpha");
    shaders_["Quasi Physical"]->AttachUniform<float>("zdecay");
    shaders_["Quasi Physical"]->AttachUniform<float>("wdecay");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("zColor");
    shaders_["Quasi Physical"]->AttachUniform<glm::vec4>("wColor");



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



}

void QuasiCrystalsScene::SetupUniforms(Camera* camera, QuasiCrystal *quacry)
{

    SetUniformInternal(camera, quacry);
    SetUniformPhysical(camera, quacry);
    SetUniformPhysicalBox(camera, quacry);
}

void QuasiCrystalsScene::SetUniformInternal(QuasiCrystalsScene::Camera *camera, QuasiCrystal *quacry)
{
    auto data = quacry->lattice_data_;

    //auto shape_transform = quacry->Shape::GetWorldTransform();
    auto shape_transform_glm  = quacry->window_->TransformWorld();


    auto transform = quacry->PointSet::GetWorldTransform();
    auto transform_glm = MakeGLM(transform);

    vec4 colorw = {0.3f,0.3f,0.3f,1.0f}; // data.w_color_;
    auto colorw_glm = MakeGLM(colorw);
    vec4 colorz =  {0.3f,0.3f,0.3f,1.0f}; //data.z_color_;
    auto colorz_glm = MakeGLM(colorz);


    auto shader = shaders_["Quasi Internal"];
    shader->SetUniform<float>("depth", data->depth_);
    shader->SetUniform<glm::mat4>("shape_transform", shape_transform_glm);

    shader->SetUniform<glm::mat4>("pv",  *camera);
    shader->SetUniform<glm::mat4>("transform", transform_glm);
    //std::vector<glm::vec2> shape_vectors_glm = vec2vecToGLM(quacry->transformed_vertices_);
    //shader->SetUniform< std::vector< glm::vec2 > >("shape", shape_vectors_glm, 8);
    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, 8, &quacry->window_->transformed_vertices_[0][0]);

    shader->SetUniform<float>("point_size", data->point_size_window_);
    shader->SetUniform<float>("alpha", 1.0f);
    shader->SetUniform<float>("zdecay", data->z_decay_);
    shader->SetUniform<float>("wdecay", data->w_decay_);
    shader->SetUniform<glm::vec4>("zColor", colorz_glm);
    shader->SetUniform<glm::vec4>("wColor", colorw_glm);
}

void QuasiCrystalsScene::SetUniformPhysical(QuasiCrystalsScene::Camera *camera, QuasiCrystal *quacry)
{
    auto data = quacry->lattice_data_;

    auto shader = shaders_["Quasi Physical"];

    GLuint shape = glGetUniformLocation(*shader, "shape");
    glUniform2fv(shape, 8, &quacry->window_->transformed_vertices_[0][0]);

    shader->SetUniform<glm::mat4>("pv", *camera);
    shader->SetUniform<glm::mat4>("transform", quacry->PointSet::GetWorldTransform());
    shader->SetUniform<float>("point_size", data->point_size_);
    shader->SetUniform<float>("alpha", data->alpha_);
    shader->SetUniform<float>("zdecay", data->z_decay_);
    shader->SetUniform<float>("wdecay", data->w_decay_);
    shader->SetUniform<glm::vec4>("zColor", data->z_color_);
    shader->SetUniform<glm::vec4>("wColor", data->w_color_);
}

void QuasiCrystalsScene::SetUniformPhysicalBox(QuasiCrystalsScene::Camera *camera, QuasiCrystal *quacry)
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

}

void QuasiCrystalsScene::Signup()
{

}

void QuasiCrystalsScene::Receive(std::shared_ptr<Event> event)
{

}

void QuasiCrystalsScene::Setup()
{

}

void QuasiCrystalsScene::Draw()
{

}

void QuasiCrystalsScene::AddQuasiCrystal(QuasiCrystal &&)
{

}

QuasiCrystal *QuasiCrystalsScene::ActiveQuasiCrystal()
{
    return active_quacry_;
}

void QuasiCrystalsScene::ActiveQuasiCrystal(QuasiCrystal *)
{

}

bool QuasiCrystalsScene::HasQuasiCrystal()
{
    return !quacries_.empty();
}








}
