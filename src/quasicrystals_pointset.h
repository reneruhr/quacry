#include "kipod.h"

namespace quacry{
using Vector4 = glm::vec4;
using Basis4 = glm::mat4;
using Vec4 = glm::vec4;
using SampleSize = std::vector< int >;

class PointSet4 : public kipod::RenderObject
{
public:
    PointSet4(SampleSize sample = {-25,25,-25,25,-10,10,-10,10}) :
	    sample_size_(sample){}
    
    PointSet4& operator=(PointSet4&&) = default;
    PointSet4(PointSet4&&) = default;

    virtual void Init();
    virtual void Draw();

    void UpdateSample(const SampleSize& sample);
protected:
    std::vector<Vector4> sample_;
private:
    SampleSize sample_size_;
    std::vector<Vector4>* MakeSample();
};
}
