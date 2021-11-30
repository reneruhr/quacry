#include "quasicrystal.h"
#include <utility>
#include "math/LLL.h"

namespace quacry{
using Mat4 = glm::mat4;
using namespace kipod::MeshModels;

Quasicrystal22::Quasicrystal22(Basis4 basis, Window2 window, std::string name, SampleSize sample)
    : Lattice(basis), PointSet4(std::move(sample)), Quasicrystal(std::move(name)),
      window_(std::make_unique<Window2>(std::move(window))),
    view_data_(std::make_unique<ViewData>()),
     g_(basis)
{
    Init();
}

Quasicrystal22::Quasicrystal22(Basis4 basis, Window2 window, SampleSize sample)
    : Lattice(basis), PointSet4(std::move(sample)),
      window_(std::make_unique<Window2>(std::move(window))),
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



bool Quasicrystal23::InsideWindow(const Vec3 &v, const Mat4 &g)
{
    LOG_DEBUG("Test Point {} {} {}", v.x, v.y ,v.z);
    auto Inside = [this, &g](const Vec3& v, int n){
        auto Apply_g = [&g](const Vec3& w) { auto ww = g*Vec4(w,1); return ww/ww.w; };
        auto Apply_g_to_T = [&Apply_g](Triangle3f& T) {  T.a = Apply_g(T.a); T.b = Apply_g(T.b); T.c = Apply_g(T.c);   };
        auto T = window_->Triangle(n);
        Apply_g_to_T(T);
        LOG_DEBUG("Test g*Triangle {} ", T);
        float det = VolumeTetrahedron(T, v);
        LOG_DEBUG("Determinant {}", det);
        return det >= 0.f;
    };
    for(int i = 0, e = window_->NumberOfTriangles(); i<e; ++i)
        if(!Inside(v, i)) return false;
    return true;
}

Quasicrystal23::Quasicrystal23(const std::string &name, const Mat5f &lattice, const Window3 &window,
                               SampleSize size_)
: RenderObject(), Quasicrystal(name), lattice_(lattice), window_temp_(std::make_unique<Window3>(window)) , view_data_(std::make_unique<ViewData>()), g_(lattice), sample_size_(std::move(size_))
{
    window_ = window_temp_.get();
    auto correct = TestIfCCWOriented(*window_);
    LOG("Testing window mesh for correct orientation: {}", correct);
    if(!correct) {
        LOG("No problem, we fix it");
        FixCCWOriented(*window_);
    }
    MakeSample();
    Init();
}

void Quasicrystal23::MakeSample()
{
    int total = (sample_size_[1]-sample_size_[0]+1)*(sample_size_[3]-sample_size_[2]+1)*(sample_size_[5]-sample_size_[4]+1)*(sample_size_[7]-sample_size_[6]+1)*(sample_size_[9]-sample_size_[8]+1);
    LOG_INFO("Creating Sample of {} elements", total);
    sample_ = std::make_unique<std::vector<Vec5f>>();
    sample_rejected_ = std::make_unique<std::vector<Vec5f>>();
    windowed_sample_ = std::make_unique<WindowedSample5>(WindowedSample5());
    auto g = window_->Transform();

    for(int x0= sample_size_[0]; x0<= sample_size_[1]; ++x0)
    for(int x1= sample_size_[2]; x1<= sample_size_[3]; ++x1)
    for(int x2= sample_size_[4]; x2<= sample_size_[5]; ++x2)
    for(int x3= sample_size_[6]; x3<= sample_size_[7]; ++x3)
    for(int x4= sample_size_[8]; x4<= sample_size_[9]; ++x4){
        Vec5f v = g_ * Vec5f(x0,x1,x2,x3,x4);
        if(InsideWindow(Vec3(v[2], v[3], v[4]), g)){
            sample_->push_back(v);
            windowed_sample_->Add({x0,x1,x2,x3,x4});
        } else
            sample_rejected_->push_back(v);
    }
    LOG_INFO("Actual sample points in window: {}", sample_->size());
    for(auto& s : *(windowed_sample_->sample_)){
        s.second = windowed_sample_->Neighbors(s.first);
    }
}

void Quasicrystal23::Init()
{
    std::string name = "Quasicrystal23";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet23(sample_.get());
    AddLayout(name, std::move(*layout));

    std::string name_edges = "Quasicrystal23 Edges";
    auto layout_edges = new kipod::GLRenderLayout;
    auto buffer10 = windowed_sample_->AsBuffer();
    layout_edges->SetupEdges23(buffer10.get());
    AddLayout(name_edges, std::move(*layout_edges));

    std::string name_rejected = "Quasicrystal23 Rejected";
    auto layout_rejected = new kipod::GLRenderLayout;
    layout_rejected->SetupPointSet23(sample_rejected_.get());
    AddLayout(name_rejected, std::move(*layout_rejected));
    /* internal_ = std::make_unique<RenderObject>();
    auto layout_internal = new kipod::GLRenderLayout;
    layout_internal->SetupPointSet23(sample_.get());
    internal_->AddLayout(name, std::move(*layout_internal));
*/
 }

void Quasicrystal23::Draw(){
    RenderObject::Draw("Quasicrystal23");
}

void Quasicrystal23::Draw(Geometry geometry = Geometry::Points)
{
    if(geometry == Geometry::Points)
        RenderObject::Draw("Quasicrystal23");
    else if(geometry == Geometry::Edges)
        RenderObject::Draw("Quasicrystal23 Edges");
    else if(geometry == Geometry::PointsMat5)
        RenderObject::Draw("Quasicrystal23 Edges");
}

void Quasicrystal23::Draw(Space space)
{
   if(space == Space::Physical || space == Space::Internal )  RenderObject::Draw("Quasicrystal23");
   else if(space == Space::Rejected)  RenderObject::Draw("Quasicrystal23 Rejected");
}

void Quasicrystal23::Resample()
{
    MakeSample();
    Relayout();
}

void Quasicrystal23::Relayout()
{
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet23(sample_.get());
    ChangeLayout("Quasicrystal23", std::move(*layout));

    auto layout_edges = new kipod::GLRenderLayout;
    auto buffer10 = windowed_sample_->AsBuffer();
    layout_edges->SetupEdges23(buffer10.get());
    ChangeLayout("Quasicrystal23 Edges", std::move(*layout_edges));

    auto layout_rejected = new kipod::GLRenderLayout;
    layout_rejected->SetupPointSet23(sample_rejected_.get());
    ChangeLayout("Quasicrystal23 Rejected", std::move(*layout_rejected));
}

//Encode Neighbors in a single Vec5i vector. x_i = {-1,0,1, 2="1"+"-1"}

auto WindowedSample5::Neighbors(const Vec5i &index) const -> std::vector<int>
{
    std::vector<int> neighbors = {0,0,0,0,0} ;
    for(int i = 0; i<5;  ++i){
        Vec5i v(0,0,0,0,0);
        v[i] = 1;
        if(Has(index - v)) neighbors[i] = -1;
        if(Has(index + v)) neighbors[i] =  1 + neighbors[i]*neighbors[i];
    }
    return neighbors;
}

bool WindowedSample5::Has(const Vec5i &index) const
{
    auto res = sample_->find(index);
    if(res!=sample_->end()) return true;
    else return false;
}

void WindowedSample5::Add(const Vec5i &index) const
{
    sample_->insert({index, {}});
}

auto WindowedSample5::AsBuffer() -> std::unique_ptr<std::vector<Vec10f>>
{
    auto sample_with_neighbors = std::make_unique<std::vector<Vec10f>>();
    for (auto &v: *sample_)
        sample_with_neighbors->emplace_back(Vec10f(v.first[0], v.first[1], v.first[2], v.first[3], v.first[4],
                                             v.second[0], v.second[1], v.second[2], v.second[3], v.second[4]));
    LOG("Created Vec10f Buffer of {} elements", sample_with_neighbors->size());
    for(const auto& v : *sample_with_neighbors){
        auto w = Vec5f(v.head<5>());
        auto t = Vec5f(v.tail<5>());
        LOG_DEBUG("Vector \n {}", w.transpose());
        LOG_DEBUG("Neighbors \n {}", t.transpose());
    }

    return sample_with_neighbors;
}
}
