#pragma once
#include "quasicrystals_pointset.h"
#include "../kipod/src/modules/shapes/shape.h"

namespace kipod::QuasiCrystals{
using Window = kipod::Shapes::Shape;



class QuasiCrystal : public PointSet
{
public:    
    QuasiCrystal(Basis basis, Window window, Sample sample);

    std::unique_ptr<Window> window_;


    virtual void Init() override;
    //virtual void Draw() override;


};

}
