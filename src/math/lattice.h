#pragma once
#include <glm/matrix.hpp>
#include <vector>
typedef glm::mat4 Mat4;
typedef glm::mat2 Mat2;
typedef glm::vec4 Vec4;
typedef glm::vec2 Vec2;
using Vector = Vec4; 
using Basis = Mat4; 
using Sample = std::vector< int >;

class Lattice
{
public:
    Basis basis_;
    std::vector<Vector> sample_;
    Sample sample_size_;

    Lattice(Sample sample_size = {-10,10,-10,10,-10,10,0,0}, Basis basis = Basis(1.0f))
        :  basis_(basis), sample_size_(sample_size) {}

    Lattice& operator=(const Lattice&) = default;
    Lattice& operator=(Lattice&&) = default;
    Lattice(const Lattice&) = default;
    Lattice(Lattice&&) = default;
    ~Lattice() = default;

    Basis GetBasis()
    {
        return basis_;
    }

    Vector operator()(float x, float y, float z, float w=0)
    {
        return x*basis_[0] + y*basis_[1] + z*basis_[2] + w*basis_[3];
    }

    std::vector<Vector>* MakeSample()
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
            // LOG_INFO("Created Sample of size {}", size(sample_));
            return &sample_;
    }
};

