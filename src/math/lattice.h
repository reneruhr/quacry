#pragma once
#include "../../kipod/src/utils/mat.h"
#include "../../kipod/src/utils/vec.h"

using Vector = vec4;
using Basis = mat4;
using Sample = std::vector< int >;

class Lattice
{
public:
    Basis basis_;
    std::vector<Vector> sample_;
    Sample sample_size_;

    Lattice(Sample sample_size = {-10,10,-10,10,-10,10,0,0}, Basis basis = Basis())
        :  basis_(basis), sample_size_(sample_size)
    {
        assert(size(sample_size) == 8);
    }

    Lattice& operator=(const Lattice&) = default;
    Lattice& operator=(Lattice&&) = default;
    Lattice(const Lattice&) = default;
    Lattice(Lattice&&) = default;

    Basis GetBasisMatrix()
    {
        return basis_;
    }




    vec4 operator()(int x, int y, int z, int w=0)
    {
        return x*basis_[0] + y*basis_[1] + z*basis_[2] + w*basis_[3];
    }

    std::vector<vec4>* MakeSample()
    {
        int X0 = sample_size_[0], X = sample_size_[1],
            Y0 = sample_size_[2], Y = sample_size_[3],
            Z0 = sample_size_[4], Z = sample_size_[5],
            W0 = sample_size_[6], W = sample_size_[7];

            for(int x=X0; x<=X; x++)
                for(int y=Y0; y<=Y; y++)
                    for(int z=Z0; z<=Z; z++)
                        for(int w=W0; w<=W; w++)
                            sample_.emplace_back((*this)(x,y,z,w));
            LOG_INFO("Created Sample of size {}", size(sample_));
            return &sample_;
    }
};

