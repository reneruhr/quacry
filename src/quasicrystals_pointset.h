#include "kipod.h"
#include "math/lattice.h"

namespace quacry{
using Vector = glm::vec4;
using Basis = glm::mat4;
using Vec4 = glm::vec4;
using Sample = std::vector< int >;

struct LatticeData{
    int size;
    float point_size_ = 5.0f;
    float point_size_window_ = 3.0f;
    float alpha_ = 0.001f;
    Vec4 color_z_ = {1.0f, 0.0f, 0.00f, 0.3f};
    Vec4 color_w_ = {0.0f, 1.0f, 0.00f, 0.3f};

    float depth_ = -0.7f;
    bool edges_ = false;
};

class PointSet : public Lattice, public kipod::RenderObject
{
public:
    PointSet(Basis basis = Basis(), Sample sample = {-10,10,-10,10,-10,10,-10,10}) :
        Lattice(sample, basis)
    {
        //LOG_INFO("Call PointSet Constructor with sample_size data {}, {} ,{}, {} ,{} ,{} ,{}, {}",
        //   sample_size_[0],sample_size_[1],sample_size_[2],sample_size_[3],sample_size_[4],sample_size_[5],
        //      sample_size_[6],sample_size_[7]);
        //LOG_INFO("...and basis {}", basis_);
    }

//    PointSet& operator=(const PointSet&) = default;
    PointSet& operator=(PointSet&&) = default;
//    PointSet(const PointSet&) = default;
    PointSet(PointSet&&) = default;

    virtual void Init();
    virtual void Draw();

    void BaseChange(Basis new_basis);
    void UpdatePoints();
    std::unique_ptr<LatticeData> lattice_data_= nullptr;
};
}
