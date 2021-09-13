#pragma once
#include "quasicrystals_pointset.h"
#include "../kipod/src/modules/shapes/shape.h"
#include "math/lattice.h"

namespace quacry{
using Window = kipod::Shapes::Shape;
struct ViewData;

class QuasiCrystal : public Lattice<Basis>, public PointSet
{
public:    
    QuasiCrystal(Basis basis, Window window);
    QuasiCrystal(Basis basis, Window window, SampleSize sample_size);
    QuasiCrystal(Basis basis, Window window, std::string name, SampleSize sample_size);

    std::unique_ptr<Window> window_;
    std::unique_ptr<ViewData> view_data_= nullptr;
    std::vector<std::unique_ptr<Window>> patterns_;

    Window* active_pattern_ = nullptr;
    virtual void Init() override;

    void ApplyLLL();
    std::string name_;
    Basis g_;
};

struct ViewData{
    int size;
    float point_size_ = 5.0f;
    float point_size_window_ = 3.0f;
    float alpha_ = 0.001f;
    Vec4 color_z_ = {1.0f, 0.0f, 0.00f, 0.3f};
    Vec4 color_w_ = {0.0f, 1.0f, 0.00f, 0.3f};

    float depth_ = -0.7f;
    bool edges_ = false;
};

}
