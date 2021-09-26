#pragma once
#include "quasicrystals_pointset.h"
#include "../kipod/src/modules/shapes/shape.h"
#include "../kipod/src/modules/meshmodels/meshmodel.h"
#include "math/lattice.h"
#include "Eigen/Dense"
namespace quacry{
using Window2 = kipod::Shapes::Shape;
using Window3 = kipod::MeshModels::MeshModel;
struct ViewData;
class WindowedSample5;


using Mat5f = Eigen::Matrix<float,5,5>;
using Vec5f = Eigen::Matrix<float,5,1>;
using namespace kipod;

struct Quasicrystal
{
    Quasicrystal() = default;
    Quasicrystal(const std::string& name) : name_(name){}
    std::string name_;
    virtual ~Quasicrystal(){}
};

class Quasicrystal22 : public Lattice<Basis4>, public PointSet4, public Quasicrystal
{
public:    
    Quasicrystal22(Basis4 basis, Window2 window);
    Quasicrystal22(Basis4 basis, Window2 window, SampleSize sample_size);
    Quasicrystal22(Basis4 basis, Window2 window, std::string name, SampleSize sample_size);

    std::unique_ptr<Window2> window_;
    std::vector<std::unique_ptr<Window2>> patterns_;

    Window2* active_pattern_ = nullptr;
    virtual void Init() override;

    std::unique_ptr<ViewData> view_data_= nullptr;

    void ApplyLLL();
    Basis4 g_;
};

enum class Space { Physical, Internal };

class Quasicrystal23 :  public RenderObject, public Quasicrystal
{
    Mat5f lattice_ = Mat5f::Identity();
    Mat5f transform_ = Mat5f::Identity();
    Mat5f g_ = Mat5f::Identity();
    std::unique_ptr<Window3> window_;
    SampleSize sample_size_ = { -5,5,  -5,5,  -5, 5, -5, 5, -2, 2 }; 
    std::unique_ptr<std::vector<Vec5f>> sample_; 
    std::unique_ptr<WindowedSample5> windowed_sample_;
    std::unique_ptr<RenderObject> internal_;

public:
    Quasicrystal23() = default;
    Quasicrystal23(const std::string& name, const Mat5f& lattice, const Window3& window);

    void Init();
    virtual void Draw() override;
    void Draw(Space);
    
    void MakeSample();
    bool InsideWindow(const Vec3& v);
    Mat5f& GetBasis() { return lattice_; };
    std::unique_ptr<ViewData> view_data_= nullptr;
};

struct ViewData{
    int size;
    float point_size_ = 5.0f;
    float point_size_window_ = 3.0f;
    float alpha_ = 0.001f;
    Vec4 color_z_ = {1.0f, 0.0f, 0.00f, 0.3f};
    Vec4 color_w_ = {0.0f, 1.0f, 0.00f, 0.3f};
    Vec4 color_window_ = Vec4(139./255,69./255,19./255,1);
    Vec4 color_pattern_ = Vec4(1.0f,0.8f,0.6f,1.0f);
    Vec4 color_ = Vec4(1.f);

    float depth_ = -0.7f;
    bool edges_ = false;
    bool window_shape_ = false;
    bool pattern_shape_ = false;
};

using Vec5i = Eigen::Matrix<int, 5, 1>;
struct VecLess{
bool operator()(const Vec5i& a, const Vec5i& b) const
{
    for(size_t i = 0; i<5; ++i)
        if(a[i] > b[i]) return false;
        else if(a[i] < b[i]) return true;
    return false;
}
};

class WindowedSample5{
    using Vec5Map = std::map<Vec5i, std::vector<int>, VecLess>;
    SampleSize* sample_size_;
    public:
    std::unique_ptr<Vec5Map> sample_;
    WindowedSample5() = default;
    WindowedSample5(SampleSize* s) : sample_size_(s), sample_(std::make_unique<Vec5Map>()){}
    void Add(const Vec5i& index){ sample_->insert({index, {}}); }
    bool Get(const Vec5i& index){ auto res = sample_->find(index);
                                   if(res!=sample_->end()) return true;
                                   else return false; } 
    auto Neighbors(const Vec5i& index) -> std::vector<int>{
        std::vector<int> neighbors = {};
        for(size_t i = 0; i<5;  ++i){
            Vec5i v(0,0,0,0,0);
            v[i] = 1;
            if(Get(index+v)) neighbors.push_back( i );
            if(Get(index-v)) neighbors.push_back(-i );
       }
      return neighbors; 
    }
};

}
