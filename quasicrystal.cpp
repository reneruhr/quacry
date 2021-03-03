#include "quasicrystal.h"
namespace kipod::QuasiCrystals{

QuasiCrystal::QuasiCrystal(Basis basis, Window window, Sample sample)
    : PointSet(basis,sample),
      window_(std::make_unique<Window>(window))
{

}

void QuasiCrystal::Init()
{
    PointSet::Init();
    window_->Init();

}

void QuasiCrystal::Draw()
{

}

}
