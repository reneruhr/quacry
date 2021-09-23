#include "quasicrystal.h"
#include "math/LLL.h"

namespace quacry{
using Mat4 = glm::mat4;

Quasicrystal22::Quasicrystal22(Basis4 basis, Window2 window, std::string name, SampleSize sample)
    : Lattice(basis), PointSet4(sample),
      window_(std::make_unique<Window2>(window)),
    view_data_(std::make_unique<ViewData>()),
    name_(name), g_(basis)
{
    Init();
}

Quasicrystal22::Quasicrystal22(Basis4 basis, Window2 window, SampleSize sample)
    : Lattice(basis), PointSet4(sample),
      window_(std::make_unique<Window2>(window)),
    view_data_(std::make_unique<ViewData>()),
    g_(basis)
{
    Init();
}

Quasicrystal22::Quasicrystal22(Basis4 basis, Window2 window)
    : Lattice(basis), PointSet4(),
      window_(std::make_unique<Window2>(window)),
    view_data_(std::make_unique<ViewData>()),
    g_(basis)
{
    Init();
}

void Quasicrystal22::Init()
{
    PointSet4::Init();
    window_->Init();
    view_data_->size = size(sample_);
    window_->world_->Replace(glm::mat2(1.));
    window_->UpdatedTransformedVertices();
    window_->UpdateShape();
}

void Quasicrystal22::ApplyLLL()
{
   using namespace latred;
   g_ = Transform() * GetBasis();
   Eigen::Map<Eigen::Matrix4f> mf(glm::value_ptr(g_),4,4); 
   Eigen::Matrix4f reduced_trans = LLL(mf);
   g_ = glm::make_mat4(reduced_trans.data());
}
 
bool Quasicrystal23::InsideWindow(const Vec3& v)
{
    auto Inside = [this](const Vec3& d, int n){
       auto [a,b,c] = window_->Triangle(n); 
       Mat4 in( Vec4(a,1), Vec4(b,1), Vec4(c,1), Vec4(d,1));
       return determinant(in) > 0;
        };
    for(int i = 0, e = window_->NumberOfTriangles(); i<e; ++i)
        if(!Inside(v, i)) return false; 
    return true;
}

Quasicrystal23::Quasicrystal23(const std::string& name, const Mat5f& lattice, const Window3& window)
: RenderObject(), name_(name), lattice_(lattice), window_(std::make_unique<Window3>(window)) , view_data_(std::make_unique<ViewData>())
{
    MakeSample();
    Init();
}

void Quasicrystal23::MakeSample()
{
    sample_ = std::make_unique<std::vector<Vec5f>>();
    for(int x0= sample_size_[0]; x0< sample_size_[1]; ++x0)
    for(int x1= sample_size_[2]; x1< sample_size_[3]; ++x1)
    for(int x2= sample_size_[4]; x2< sample_size_[5]; ++x2)
    for(int x3= sample_size_[6]; x3< sample_size_[7]; ++x3)
    for(int x4= sample_size_[8]; x4< sample_size_[9]; ++x4){
        Vec5f v = g_ * Vec5f(x0,x1,x2,x3,x4);
        if(InsideWindow(Vec3(v[2],v[3],v[4]))) sample_->push_back(v);
    }
}

void Quasicrystal23::Init()
{
    std::string name = "Quasicrystal23";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet23(sample_.get());
    AddLayout(name, std::move(*layout));

    internal_ = std::make_unique<RenderObject>(); 
    auto layout_internal = new kipod::GLRenderLayout;
    layout_internal->SetupPointSet23(sample_.get());
    internal_->AddLayout(name, std::move(*layout_internal));
}

void Quasicrystal23::Draw()
{
   RenderObject::Draw("Quasicrystal23");
}

void Quasicrystal23::Draw(Space space)
{
   if(space == Space::Physical)  RenderObject::Draw("Quasicrystal23");
   else if(space == Space::Internal)  internal_->Draw("Quasicrystal23");
}


}
