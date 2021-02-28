#pragma once
#include "quasicrystals_pointset.h"
#include "../shapes/shape.h"

namespace kipod::QuasiCrystals{
class QuasiCrystal : public PointSet
{
    using Window = kipod::Shapes::Shape;
    using Vector = vec4;
    using Basis = mat4;

public:
    QuasiCrystal(Basis basis, Window window);

    std::unique_ptr<Window> window_;

    virtual void Init() override;
    virtual void Draw() override;


    void SetOutsideVisibility(float alpha){
        lattice_data_->alpha_ = alpha;
    }
    void SetColorZW(float* z, float* w){
        lattice_data_->z_color_ = vec4(z);
        lattice_data_->w_color_ = vec4(w);
    }
};

}
