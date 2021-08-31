#include "core.h"
#include "scene.h"
#include "modules/meshmodels/meshmodel.h"

#include "quacry.h"


using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void Scene::Setup()
{
    LOG_ENGINE("Seting up MeshModel Scene.");

    name_ = "Quasicrystals";


    Camera* cam = new Camera(45, float(width_)/height_, 0.1f, 200.0);
    cam->createFrustum(); // Needed for very first Camera
    addCamera(cam);

    Light* light = new Light(kipod::LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0));
    addLight(light);
    Light* light1 = new Light(kipod::LightSource::DIFFUSE, vec4(10.0f,1.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0));
    addLight(light1);
    Light* light2 = new Light(kipod::LightSource::SPECULAR, vec4(0.0f,1.0f,10.0f,1.0f), vec4(1.0f));
    addLight(light2);

    SetupShaders();

//    std::string name = "Colored Triangles";
//    auto layout = new kipod::GLRenderLayout;
//    layout->sha_ = shaders_["Colored Triangles"];
//    boundingBox.AddLayout(name,std::move(*layout));
//    boundingBox.Init(false,false);
//    {
//        auto normal_layout = new kipod::GLRenderLayout(*layout);
//        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
//        ebo->primitive_ = GL_POINTS;
//        normal_layout->ebo_ = ebo;
//        normal_layout->sha_ = &shaders_["Normals Triangles"];
//        boundingBox.AddLayout({"Normals Triangles", normal_layout});
//    }

    SetupOptions();
    Signup();
}


void Scene::SetupOptions(){
    Add(kipod::ModeToggle("Normals", false));
    Add(kipod::ModeToggle("Camera Mode", false));
    Add(kipod::ModeToggle("Camera Frustum Mode", false));
    Add(kipod::ModeToggle("Color Mode", true));
    Add(kipod::ModeToggle("Texture Mode", true));
    Add(kipod::ModeToggle("Bounding Box", false));
    Add(kipod::ModeToggle("Wireframe", false));
    Add(kipod::ModeToggle("Emissive Mode",false));
    Add(kipod::ModeToggle("Clipping Mode", true));
}

void Scene::DrawGui()
{
//    using namespace kipod;


//    Gui::BeginWindow("MeshModels");
//    for(auto& [name,toggle] : mode_toggles_)
//        Gui::Checkbox(toggle);

//    for(auto model : models)
//        Gui::Transform(*model->world_);

//    Gui::EndWindow();

}


void Scene::AddPointSet(PointSet *point_set)
{
    point_sets_.push_back(point_set);
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::draw()
{
	mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
	mat4 v = cameras[activeCamera]->getcTransform();
	mat4 camMatrix = p*v;

    framebuffer_->Bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto point_set : point_sets_){
        glEnable( GL_BLEND );

        if(point_set->lattice_data_->qc.window == WindowType::Octagon){
            shaders_["Quasi Physical"].Use();
            SetUniformPhysical(cameras[activeCamera], (QuaCry*)point_set);
        }
        else if(point_set->lattice_data_->qc.window == WindowType::Box){
            SetupBlockUniform((QuaCry*)point_set); //NEEDS TO BE MOVED
            shaders_["Quasi Physical Box"].Use();
            SetUniformPhysicalBox(cameras[activeCamera], (QuaCry*)point_set);
        }
        point_set->PointSet::Draw();

        glEnable(GL_DEPTH_TEST);

        kipod::Shapes::Shape* shape = (kipod::Shapes::Shape*)(QuaCry*)point_set;
        shaders_["Shape"].Use();
        SetShapeUniform(shape);
        shape->kipod::Shapes::Shape::Draw();

        shaders_["Quasi Internal"].Use();
        SetUniformInternal(cameras[activeCamera], (QuaCry*)point_set);
        point_set->PointSet::Draw();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }


    kipod::RenderManager::Bind(0);

	for(auto model : models){
        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);

//        if( Toggle("Texture Mode") && model->HasLayout("Textured Triangles") ){
//           shaders_["Textured Triangles"].Use();
//           SetUniformTex(lights, cameras[activeCamera], model);
//           model->RenderObject::Draw("Textured Triangles");
//        }
//        else if((Toggle("Color Mode") || Toggle("Emissive Mode") )&& model->HasLayout("Colored Triangles")  ){
//            shaders_["Colored Triangles"].Use();
//            SetUniform(lights, cameras[activeCamera], model);
//            model->RenderObject::Draw("Colored Triangles");
//        }

        glDisable(GL_DEPTH_TEST);

        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//        if(Toggle("Normals") && model->HasLayout("Normals Triangles") ){
//            shaders_["Normals Triangles"].Use();
//            SetUniformNormal(model, cameras[activeCamera]);
//            model->RenderObject::Draw("Normals Triangles");
//        }

        if(Toggle("Bounding Box")){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            SetUniformBox(model);
            boundingBox.RenderObject::Draw("Colored Triangles");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}
    if(Toggle("Camera Mode")){
        shaders_["Basic"].Use();
		for(auto cam : cameras){
			mat4 mvp = camMatrix * Translate((cam->getEye()));
            shaders_["Basic"].SetUniform<glm::mat4>("mvp", MakeGLM(mvp));
			cam->draw();
		}
	}
    if(Toggle("Camera Frustum Mode")){
        shaders_["Basic"].Use();
		for(auto cam : cameras){
			mat4 mvp = camMatrix * Inverse(cam->getcTransform()) * Inverse(cam->getProjection(true));
            shaders_["Basic"].SetUniform<glm::mat4>("mvp", MakeGLM(mvp));
			cam->drawFrustum();
		}
	}


}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SHADER  SETUP                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::SetupShaders(){
    SetupShaderTexturedTriangles();
    SetupShaderColoredTriangles();
    SetupShaderNormals();
    void SetupShaderBasic();
    SetupShaderShape();
    SetupShaderPointSet();

    SetupShaderQuasi();

}

void Scene::SetupShaderQuasi()
{
    shaders_.insert({"Quasi Physical", kipod::Shader("inside_polygon.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Physical Box", kipod::Shader("points.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Internal", kipod::Shader("inside_polygon_window.vert.glsl", "points.frag.glsl")});
    shaders_.insert({"Quasi Physical Texture", kipod::Shader("inside_polygon.vert.glsl", "renderToTexture.frag.glsl")});


    shaders_["Quasi Physical"].AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Physical"].AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Physical"].AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Physical"].AttachUniform<float>("point_size");
    shaders_["Quasi Physical"].AttachUniform<float>("alpha");
    shaders_["Quasi Physical"].AttachUniform<float>("zdecay");
    shaders_["Quasi Physical"].AttachUniform<float>("wdecay");
    shaders_["Quasi Physical"].AttachUniform<glm::vec4>("zColor");
    shaders_["Quasi Physical"].AttachUniform<glm::vec4>("wColor");



    shaders_["Quasi Internal"].AttachUniform<float>("depth");
    shaders_["Quasi Internal"].AttachUniform<glm::mat4>("shape_transform");

    shaders_["Quasi Internal"].AttachUniform<glm::mat4>("pv");
    shaders_["Quasi Internal"].AttachUniform<glm::mat4>("transform");
    shaders_["Quasi Internal"].AttachUniform<glm::vec2>("shape");
    shaders_["Quasi Internal"].AttachUniform<float>("point_size");
    shaders_["Quasi Internal"].AttachUniform<float>("alpha");
    shaders_["Quasi Internal"].AttachUniform<float>("zdecay");
    shaders_["Quasi Internal"].AttachUniform<float>("wdecay");
    shaders_["Quasi Internal"].AttachUniform<glm::vec4>("zColor");
    shaders_["Quasi Internal"].AttachUniform<glm::vec4>("wColor");
}

void Scene::SetupShaderPointSet(){
    shaders_.insert({"PointSet", kipod::Shader("points.vert.glsl", "points.frag.glsl")});
}

void Scene::SetupShaderTexturedTriangles()
{
    shaders_.insert({"Textured Triangles", kipod::Shader("textures.vert.glsl", "textures.frag.glsl")});

    shaders_["Textured Triangles"].AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Texture Shader");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("v");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("mv");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("mv_normal");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(shaders_["Textured Triangles"]);

        shaders_["Textured Triangles"].AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(shaders_["Textured Triangles"], i);
        }
}

void Scene::SetupShaderColoredTriangles()
{
    shaders_.insert({"Colored Triangles", kipod::Shader("lights.vert.glsl",   "lights.frag.glsl")});

    shaders_["Colored Triangles"].AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Shader Colored Triangles");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("v");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("mv");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("mv_normal");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(shaders_["Colored Triangles"]);

        shaders_["Colored Triangles"].AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(shaders_["Colored Triangles"], i);
        }
}

void Scene::SetupShaderNormals()
{
    shaders_.insert({"Normals Triangles", kipod::Shader("normals.vert.glsl", "normals.frag.glsl", "normals.geom.glsl")});
    auto shader = shaders_["Normals Triangles"];
    shader.AttachUniform<float>("normal_length");
    shader.AttachUniform<glm::mat4>("mv");
    shader.AttachUniform<glm::mat4>("mv_normal");
    shader.AttachUniform<glm::mat4>("projection");
}

void Scene::SetupShaderBasic()
{
    shaders_.insert({"Basic", kipod::Shader("basic.vert.glsl",   "basic.frag.glsl")});
    shaders_["Basic"].AttachUniform<glm::mat4>("mvp");
}

void Scene::SetupShaderShape()
{
    shaders_.insert({"Shape", kipod::Shader("shape.vert.glsl",   "shape.frag.glsl")});
    shaders_["Shape"].AttachUniform<float>("depth");
    shaders_["Shape"].AttachUniform<glm::mat4>("transform");
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//QUASICRYSTAL UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::SetUniformInternal(Camera* camera, QuaCry* quacry)
{
    auto data = quacry->lattice_data_;

    //auto shape_transform = quacry->Shape::GetWorldTransform();
    auto shape_transform_glm  = quacry->Shape::TransformWorld();

    auto camp = camera->getProjection(camerasMode[activeCamera]);
    auto camc = camera->getcTransform();
    glm::mat4 p = MakeGLM(camp);
    glm::mat4 v = MakeGLM(camc);

    auto transform = quacry->PointSet::GetWorldTransform();
    auto transform_glm = MakeGLM(transform);

    vec4 colorw = {0.3f,0.3f,0.3f,1.0f}; // data.w_color_;
    auto colorw_glm = MakeGLM(colorw);
    vec4 colorz =  {0.3f,0.3f,0.3f,1.0f}; //data.z_color_;
    auto colorz_glm = MakeGLM(colorz);


    auto shader = shaders_["Quasi Internal"];
    shader.SetUniform<float>("depth", data->depth_);
    shader.SetUniform<glm::mat4>("shape_transform", shape_transform_glm);

    shader.SetUniform<glm::mat4>("pv", p*v);
    shader.SetUniform<glm::mat4>("transform", transform_glm);
    //std::vector<glm::vec2> shape_vectors_glm = vec2vecToGLM(quacry->transformed_vertices_);
    //shader.SetUniform< std::vector< glm::vec2 > >("shape", shape_vectors_glm, 8);
    GLuint shape = glGetUniformLocation(shader, "shape");
    glUniform2fv(shape, 8, &quacry->transformed_vertices_[0][0]);

    shader.SetUniform<float>("point_size", data->point_size_window_);
    shader.SetUniform<float>("alpha", 1.0f);
    shader.SetUniform<float>("zdecay", data->z_decay_);
    shader.SetUniform<float>("wdecay", data->w_decay_);
    shader.SetUniform<glm::vec4>("zColor", colorz_glm);
    shader.SetUniform<glm::vec4>("wColor", colorw_glm);
}

void Scene::SetUniformPhysical(Camera *camera, QuaCry *quacry)
{
    auto data = quacry->lattice_data_;

    auto camp = camera->getProjection(camerasMode[activeCamera]);
    auto camc = camera->getcTransform();

    auto shader = shaders_["Quasi Physical"];

    GLuint shape = glGetUniformLocation(shader, "shape");
    glUniform2fv(shape, 8, &quacry->transformed_vertices_[0][0]);

    shader.SetUniform<glm::mat4>("pv", camp*camc);
    shader.SetUniform<glm::mat4>("transform", quacry->PointSet::GetWorldTransform());
    shader.SetUniform<float>("point_size", data->point_size_);
    shader.SetUniform<float>("alpha", data->alpha_);
    shader.SetUniform<float>("zdecay", data->z_decay_);
    shader.SetUniform<float>("wdecay", data->w_decay_);
    shader.SetUniform<glm::vec4>("zColor", data->z_color_);
    shader.SetUniform<glm::vec4>("wColor", data->w_color_);
}

void Scene::SetUniformPhysicalBox(Camera *camera, QuaCry *quacry)
{
    auto data = quacry->lattice_data_;
    auto window_size = quacry->window_size_;

    auto camp = camera->getProjection(camerasMode[activeCamera]);
    auto camc = camera->getcTransform();

    auto shader = shaders_["Quasi Physical Box"];

    shader.SetUniform<glm::mat4>("pv", camp*camc);
    shader.SetUniform<glm::mat4>("transform", quacry->PointSet::GetWorldTransform());



    glBindBuffer(GL_UNIFORM_BUFFER, data->u_buffer);
    glBufferData(GL_UNIFORM_BUFFER, size(window_size)*sizeof(float), window_size.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, data->u_binding_point, data->u_buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//SETTING UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::SetUniform(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
//    auto shader = shaders_["Colored Triangles"];
//    BindLightUniforms(shader, lights);
//    BindMatrixUniforms(shader, *model, *camera);
//    BindMaterialUniforms(shader, *(model->mat_));
}

//void Scene::SetUniformNormal(MeshModel* model, Camera* camera)
//{
//    kipod::Shader* shader = &shaders_["Normals Triangles"];
//    BindMatrixUniforms(*shader, *model, *camera);
//    BindNormalUniforms(*shader, model->normal_length);
//}

void Scene::SetUniformTex(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
//   kipod::Shader* shader = &shaders_["Textured Triangles"];
//   BindTextureUniforms(*shader, model->tex_.get());
//   BindLightUniforms(*shader, lights);
//   BindMatrixUniforms(*shader, *model, *camera);
//   BindMaterialUniforms(*shader, *(model->mat_));
}

void Scene::SetUniformBox(MeshModel* model)
{
//    glm::mat4 box = model->TansformBoundingBox();
//    mat4 camc = cameras[activeCamera]->getcTransform();
//    glm::mat4 mv = MakeGLM(camc) * box;
//    mat4 camp = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
//    glm::mat4 p = MakeGLM(camp);

//    shaders_["Colored Triangles"].SetUniform<glm::mat4>("mv", mv);
//    shaders_["Colored Triangles"].SetUniform<glm::mat4>("projection", p);
}

void Scene::SetShapeUniform(kipod::Shapes::Shape* shape)
{
//    mat4 shape_matrix = shape->GetWorldTransform();
//    auto transform = MakeGLM(shape_matrix);

//    shaders_["Shape"].SetUniform<glm::mat4>("transform", transform);
//    shaders_["Shape"].SetUniform<float>("depth", shape->depth_);
}

void Scene::SetupBlockUniform(QuaCry *quacry){
    auto data = quacry->lattice_data_;
    auto shader = shaders_["Quasi Physical Box"];

    data->u_block_index = glGetUniformBlockIndex(shader, "WindowBlock");
    glUniformBlockBinding(shader, data->u_block_index, data->u_binding_point);

    glGenBuffers(1, &data->u_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, data->u_buffer);
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING UNIFORMS                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void Scene::BindLightUniforms(kipod::Shader& shader, vector<Light *> &lights)
{
    for(int i = 0; i<3; ++i){
            SetLightToShader(shader, i, lights[i]);
    }
}

void Scene::BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material)
{
    SetMaterialToShader(shader, material);
}

void Scene::BindTextureUniforms(kipod::Shader& shader, const kipod::Texture* texture)
{
    shader.SetUniform<float>(texture->name_.c_str(), 0.0f);
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
}

void Scene::BindNormalUniforms(kipod::Shader& shader, const float length)
{
    shader.SetUniform<float>("normal_length", length);
}

void Scene::BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject& model, const Camera& camera)
{
    glm::vec4  camLocation = glm::vec4(camera.eye_,1);
    glm::mat4 p = camera.projection_matrix_;
    glm::mat4 v = camera.view_matrix_;

    shader.SetUniform<glm::vec4>("cameraLocation", camLocation);
    shader.SetUniform<glm::mat4>("v", v);
    shader.SetUniform<glm::mat4>("projection", p);

    auto m = model.Transform();

    glm::mat4  mv = v*m;
    glm::mat4 mv_normal = glm::transpose(glm::inverse(mv));

    shader.SetUniform<glm::mat4>("mv", mv);
    shader.SetUniform<glm::mat4>("mv_normal", mv_normal);

}




/////////////////////////////////////////////////////////////////////////////////////////////////
//   MODEL CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////





void Scene::LoadOBJModel(string fileName, bool textured)
{
//    MeshModel *model = new MeshModel(fileName, textured);

//    model->SetUniformMaterial();
//    models.push_back(model);

//     //AddModel(model); Broke when changing render_scene
//    std::string name;
//    bool foundTexture;
//    name = model->tex_ ? (LOG_ENGINE("A Texture was set. Use Tex Shader"), foundTexture=true,  "Textured Triangles")
//                       : (LOG_ENGINE("No Texture set. Use Light Shader."), foundTexture=false, "Colored Triangles" );
//    auto layout = new kipod::GLRenderLayout;
//    layout->sha_ = &shaders_[name];
//    LOG_ENGINE("Add Layout name {}", name);
//    model->AddLayout(name, std::move(*layout));
//    model->Init(foundTexture);

//    if(!foundTexture){
//        auto normal_layout = new kipod::GLRenderLayout(*layout);
//        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
//        ebo->primitive_ = GL_POINTS;
//        normal_layout->ebo_ = ebo;
//        normal_layout->sha_ = &shaders_["Normals Triangles"];
//        model->AddLayout({"Normals Triangles", normal_layout});
//    }

}

void Scene::LoadPrimitive(kipod::MeshModels::Primitive primitive, int numberPolygons)
{
//    PrimMeshModel *model = new PrimMeshModel(primitive, numberPolygons);
//    model->SetUniformMaterial();
//    models.push_back(model);

//    //AddModel(model); Broke when changing render_scene
//    std::string name = "Colored Triangles";
//    auto layout = new kipod::GLRenderLayout;
//    layout->sha_ = &shaders_["Colored Triangles"];
//    model->AddLayout(name, std::move(*layout));
//    model->Init(false);
//    {
//        auto normal_layout = new kipod::GLRenderLayout(*layout);
//        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
//        ebo->primitive_ = GL_POINTS;
//        normal_layout->ebo_ = ebo;
//        normal_layout->sha_ = &shaders_["Normals Triangles"];
//        model->AddLayout({"Normals Triangles", normal_layout});
//    }
}



//void Scene::moveModel(int model_id, const vec3& translate){
//    if(numberOfModels() <= model_id) return;
//    MeshModel* model = models[model_id];
//    model->move(translate);
//    needs_update = true;
//}

//void Scene::lookAtModel(int camera_id, int model_id){
//    if(numberOfModels() <= model_id) return;
//    if(numberOfCameras() <= camera_id) return;
//    Camera* cam =cameras[camera_id];
//    MeshModel* model = models[model_id];
//    cam->updateAt(model->getCenter());
//}




/////////////////////////////////////////////////////////////////////////////////////////////////
//Shape   CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::AddShape(kipod::Shapes::Shape *shape)
{
    shapes_.push_back(shape);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//LIGHT  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::addLight(Light *light)
{
    lights.push_back(light);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//CAMERA  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::addCamera(Camera *cam, bool projective)
{
    cameras.push_back(cam);
    camerasMode.push_back(projective);
}

void Scene::setCameraMode(int camera_id, bool perspective)
{
    camerasMode[camera_id] = perspective;
}

void Scene::setLastCameraActive()
{
    if(cameras.empty()) return;
    activeCamera = cameras.size()-1;
}

Camera* Scene::getActiveCamera()
{
    return cameras[activeCamera];
}

MeshModel* Scene::getActiveModel(){
    if(activeModel>=0) return models[activeModel];
    else return nullptr;
}

void Scene::moveCamera(int camera_id, const vec3& eye, const vec3& at, const vec3& up )
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam = cameras[camera_id];
    cam->updateLookAt( eye, at, up );
}

void Scene::moveEyeOfCamera(int camera_id, const vec3& eye)
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam = cameras[camera_id];
    cam->moveEye(eye);
}

void Scene::perspectiveCamera(int camera_id, const float& fovy, const float& aspect, const float& near_new, const float& far_new)
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam =cameras[camera_id];
    cam->Perspective(fovy, aspect, near_new, far_new);
    needs_update = true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//SOFTRENDER                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void Scene::drawSoft()
{
//    mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
//    mat4 v = cameras[activeCamera]->getcTransform();
//    _softrenderer->SetCameraTransform(v);
//    _softrenderer->SetProjection(p);
//    for(auto model : models){
//        mat4 m = model->getmTransform();
//        mat3 n = mat3(vec3(&m[0][0]),vec3(&m[1][0]),vec3(&m[2][0]));
//        n = transpose(Inverse(n));
//        _softrenderer->SetObjectMatrices(m,n);

//        if(color_mode || emissive_mode)
//            model->drawWithLight(_softrenderer,lights, color_mode, emissive_mode);
//        else
//            model->draw(_softrenderer, wireframemode, clipping_mode, normals_mode);

//        if(box_mode){
//             mat4 m = mat4( &model->TansformBoundingBox()[0][0] ); //to be changed
//             _softrenderer->SetObjectMatrices(m, mat3(1.0));
//             boundingBox.draw(_softrenderer, true,false);
//        }
//    }
}

void Scene::drawDemo()
{
    _softrenderer->SetDemoBuffer();
}


void Scene::clearBuffer()
{
    _softrenderer->ClearBuffer();
}

void Scene::swapBuffers()
{
    _softrenderer->SwapBuffers();
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//EVENTS                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void Scene::processEvent(Event& event){
    if(event.getType() == EventType::TranslateCamera){
        needs_update = true;
        if(event.getData().direction == MoveDirection::RIGHT)   {
            moveEyeOfCamera(activeCamera, vec3(1,0,0));
            return;
        }
        if(event.getData().direction == MoveDirection::LEFT){
            moveEyeOfCamera(activeCamera, vec3(-1,0,0));
        return;
        }
        if(event.getData().direction == MoveDirection::UP) {
            moveEyeOfCamera(activeCamera, vec3(0,1,0));
        return;
        }
        if(event.getData().direction == MoveDirection::DOWN) {
            moveEyeOfCamera(activeCamera, vec3(0,-1,0));
        return;
        }
        if(event.getData().direction == MoveDirection::FORWARD)  {
            moveEyeOfCamera(activeCamera, vec3(0,0,1));
        return;
        }
        if(event.getData().direction == MoveDirection::BACKWARD){
            moveEyeOfCamera(activeCamera, vec3(0,0,-1));
        return;
        }
    }
    if(event.getType() == EventType::TranslateModel){
        needs_update = true;
//        if(event.getData().direction == MoveDirection::RIGHT)  { moveModel(activeModel, vec3(1,0,0)); return; }
//        if(event.getData().direction == MoveDirection::LEFT) {  moveModel(activeModel, vec3(-1,0,0)); return; }
//        if(event.getData().direction == MoveDirection::UP)  { moveModel(activeModel, vec3(0,1,0)); return; }
//        if(event.getData().direction == MoveDirection::DOWN) {  moveModel(activeModel, vec3(0,-1,0)); return; }
//        if(event.getData().direction == MoveDirection::FORWARD)  { moveModel(activeModel, vec3(0,0,1)); return; }
//        if(event.getData().direction == MoveDirection::BACKWARD)  { moveModel(activeModel, vec3(0,0,-1)); return; }
    }

//    if(event.getType() == EventType::RotateModel){
//        MeshModel* model = getActiveModel();
//        needs_update = true;
//        if(event.getData().rotateDiraction == RotateDirection::ROLL)  { model->moveLocal(RotateX(event.getData().amount)); return; }
//        if(event.getData().rotateDiraction == RotateDirection::YAW) { model->moveLocal(RotateY(event.getData().amount)); return; }
//        if(event.getData().rotateDiraction == RotateDirection::PITCH)  { model->moveLocal(RotateZ(event.getData().amount)); return; }
//    }
    if(event.getType() == EventType::SceneUpdate){
        needs_update = true;
        return;
    }
    if(event.getType() == EventType::WireframeMode){
        LOG_DEBUG("Switched Wireframe");
        wireframemode = !wireframemode;
        color_mode = !wireframemode;
        emissive_mode = !wireframemode;
        return;
    }
    if(event.getType() == EventType::EmissiveMode){
        emissive_mode = !emissive_mode;
        return;
    }
    if(event.getType() == EventType::LightMode){
        color_mode = !color_mode;
        return;
    }
    if(event.getType() == EventType::BoxMode){
        LOG_DEBUG("Switched Bounding Box");
        box_mode = !box_mode;
        return;
    }
    if(event.getType() == EventType::NormalMode){
        LOG_DEBUG("Switched Normal Mode");
        normals_mode = !normals_mode;
        return;
    }
    if(event.getType() == EventType::CameraMode){
        LOG_DEBUG("Camera Bounding Box");
        camera_mode = !camera_mode;
        return;
    }
    if(event.getType() == EventType::ClippingMode){
        clipping_mode = !clipping_mode;
        return;
    }
    if(event.getType() == EventType::TextureMode){
        texture_mode = !texture_mode;
        return;
    }
}


void Scene::ProcessKeys(kipod::KeyPressedEvent &event)
{
        float stepsize = 1.0f;
        auto key = event.GetKeyCode();


        //TOGGLES:
        if(key == Key::Space)
            Toggle("Wireframe").Switch();
        else if(key == Key::N)
            Toggle("Normals").Switch();
        else if(key == Key::B)
            Toggle("Bounding Box").Switch();
        else if(key == Key::C)
            Toggle("Camera Mode").Switch();
        else if(key == Key::X)
            Toggle("Clipping Mode").Switch();
        //TRANSFORM CONTROL:
//        else if(key == Key::Left){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({-stepsize,0,0});
//                }
//        else if(key == Key::Right){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({+stepsize,0,0});
//                }
//        else if(key == Key::Up){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({0,stepsize,0});
//                }
//        else if(key == Key::Down){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({0,-stepsize,0});
//                }
//        else if(key == Key::PageUp){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({0,0, stepsize});
//                }
//        else if(key == Key::PageDown){
//                if(getActiveModel())
//                    getActiveModel()->world_->Translate({0,0,-stepsize});
//                }
        //CAMERA CONTROL:
        else if(key == Key::A){
                    getActiveCamera()->Move({-stepsize,0,0});
                }
        else if(key == Key::D){
                    getActiveCamera()->Move({+stepsize,0,0});
                }
        else if(key == Key::W){
                    getActiveCamera()->Move({0,stepsize,0});
                }
        else if(key == Key::S){
                    getActiveCamera()->Move({0,-stepsize,0});
                }
        else if(key == Key::R){
                    getActiveCamera()->Move({0,0, stepsize});
                }
        else if(key == Key::F){
                    getActiveCamera()->Move({0,0,-stepsize});
                }

}
