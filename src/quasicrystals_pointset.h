#include "kipod.h"

namespace quacry{
using Vector = glm::vec4;
using Basis = glm::mat4;
using Vec4 = glm::vec4;
using SampleSize = std::vector< int >;

class PointSet : public kipod::RenderObject
{
public:
    PointSet(SampleSize sample = {-25,25,-25,25,-10,10,-10,10}) :
	    sample_size_(sample){}
    
    PointSet& operator=(PointSet&&) = default;
    PointSet(PointSet&&) = default;

    virtual void Init();
    virtual void Draw();

    void UpdateSample(const SampleSize& sample);
protected:
    std::vector<Vector> sample_;
private:
    SampleSize sample_size_;
    std::vector<Vector>* MakeSample();
};
}
