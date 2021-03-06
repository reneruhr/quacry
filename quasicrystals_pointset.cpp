#include "quasicrystals_pointset.h"

namespace kipod::QuasiCrystals{

void PointSet::Init()
{
    LOG_INFO("Initialize PointSet");
    MakeSample();

    std::string name = "PointSet";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet(&sample_);
    AddLayout(name, std::move(*layout));

    lattice_data_ =  std::make_unique<LatticeData>();
    lattice_data_->size = size(sample_);
}

void PointSet::Draw()
{
   RenderObject::Draw("PointSet");
}


void PointSet::BaseChange(Basis new_basis){
    basis_ = new_basis;
    sample_= {};
    MakeSample();
    UpdatePoints();
}

void PointSet::UpdatePoints()
{
    unsigned long buffer_size = size(sample_) * sizeof(Vector);
    static_cast<GLRenderLayout*>(Layout())->vbo_->Add(0, buffer_size, (void*)sample_.data());
    lattice_data_->size = size(sample_);
}

}
