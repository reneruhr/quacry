#include "quasicrystals_pointset.h"

namespace kipod::QuasiCrystals{

void PointSet::Init()
{
    LOG_ENGINE("Initialize PointSet");
    MakeSample();

    std::string name = "PointSet";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet(&sample_);
    AddLayout(name, std::move(*layout));

    lattice_data_ =  std::make_shared<LatticeData>();
    lattice_data_->size = size(sample_);
}

void PointSet::Draw()
{
   RenderObject::Draw("PointSet");
}


void PointSet::BaseChange(mat4 new_basis){
    basis_ = new_basis;
    sample_= {};
    MakeSample();
    UpdatePoints();
}

void PointSet::UpdatePoints()
{
    lattice_data_->size = size(sample_);

    glBindVertexArray(lattice_data_->vao);
    glBindBuffer(GL_ARRAY_BUFFER, lattice_data_->vbo);
    glBufferData(GL_ARRAY_BUFFER, lattice_data_->size * 4 * sizeof(float), sample_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

}
