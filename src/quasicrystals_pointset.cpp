#include "quasicrystals_pointset.h"

namespace quacry{

void PointSet4::Init()
{
    MakeSample();

    std::string name = "PointSet";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet(&sample_);
    AddLayout(name, std::move(*layout));
}

void PointSet4::Draw()
{
   RenderObject::Draw("PointSet");
}

void PointSet4::UpdateSample()
{
    sample_.clear();
    MakeSample();
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet(&sample_);
    ChangeLayout("PointSet", std::move(*layout));
}

std::vector<Vector4>* PointSet4::MakeSample()
{
	int X0 = sample_size_[0], X = sample_size_[1],
    Y0 = sample_size_[2], Y = sample_size_[3],
    Z0 = sample_size_[4], Z = sample_size_[5],
    W0 = sample_size_[6], W = sample_size_[7];

    for(int x=X0; x<=X; x++)
	    for(int y=Y0; y<=Y; y++)
		    for(int z=Z0; z<=Z; z++)
			    for(int w=W0; w<=W; w++)
				    sample_.emplace_back(Vector4(x,y,z,w));
    return &sample_;
}
}
