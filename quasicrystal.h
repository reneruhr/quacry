#pragma once
#include "quasicrystals_pointset.h"
#include "../shapes/shape.h"

namespace kipod::QuasiCrystals{
using Window = kipod::Shapes::Shape;



class QuasiCrystal : public PointSet
{
public:    
    QuasiCrystal(Basis basis, Window window, Sample sample);

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
