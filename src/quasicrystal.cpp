#include "quasicrystal.h"
#include "math/LLL.h"

namespace quacry{
using Mat4 = glm::mat4;

QuasiCrystal::QuasiCrystal(Basis basis, Window window, std::string name, SampleSize sample)
    : Lattice(basis), PointSet(sample),
      window_(std::make_unique<Window>(window)),
    view_data_(std::make_unique<ViewData>()),
    name_(name), g_(basis)
{
    Init();
}

QuasiCrystal::QuasiCrystal(Basis basis, Window window, SampleSize sample)
    : Lattice(basis), PointSet(sample),
      window_(std::make_unique<Window>(window)),
    view_data_(std::make_unique<ViewData>()),
    g_(basis)
{
    Init();
}

QuasiCrystal::QuasiCrystal(Basis basis, Window window)
    : Lattice(basis), PointSet(),
      window_(std::make_unique<Window>(window)),
    view_data_(std::make_unique<ViewData>()),
    g_(basis)
{
    Init();
}

void QuasiCrystal::Init()
{
    PointSet::Init();
    window_->Init();
    view_data_->size = size(sample_);
    window_->world_->Replace(glm::mat2(1.));
    window_->UpdatedTransformedVertices();
    window_->UpdateShape();
}

void QuasiCrystal::ApplyLLL()
{
   using namespace latred;
   g_ = Transform() * GetBasis();
   Eigen::Map<Eigen::Matrix4f> mf(glm::value_ptr(g_),4,4); 
   Eigen::Matrix4f reduced_trans = LLL(mf);
   g_ = glm::make_mat4(reduced_trans.data());
}
}
