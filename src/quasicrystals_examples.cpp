#include "quasicrystals_examples.h"
#include <Eigen/Geometry>
namespace quacry{
using Vec2 = glm::vec2;
using namespace kipod::Shapes;

auto FindIntersection(const Vec2& a,const Vec2& b, const Vec2& x, const Vec2& y) -> Vec2
{
    using Line2 = Eigen::Hyperplane<float,2>;
    auto ToEigen = [](Vec2 x){ return Eigen::Vector2f{x.x,x.y}; };
    Line2 A = Line2::Through(ToEigen(a), ToEigen(b));
    Line2 B = Line2::Through(ToEigen(x), ToEigen(y));
    Eigen::Vector2f p = A.intersection(B);
    return {p[0],p[1]};
}

auto GetOrthogonalLine(const Vec2&a, const Vec2& b) -> Vec2
{
    Vec2 dir = b-a;
    return {-dir.y, dir.x};
}

auto Pattern1(float s) ->std::vector<Vec2>
{
    auto o = Octagon(s);
    auto a = o.GetVertex(6);
    auto b = o.GetVertex(7);
    auto c = FindIntersection(b, 
                              b+GetOrthogonalLine(b, o.GetVertex(0)),
                              a,
                              a+GetOrthogonalLine(o.GetVertex(5),a)); 
    return {a,b,c};
}


auto AmmannBeenker() -> QuasiCrystal 
{
    const float s = std::sqrt(2);

    Basis basis = {1,0,s,0,
                   1,2,0,s,
                   1,0,-s,0,
                   1,2,0,-s}; basis = glm::transpose(basis);

    SampleSize sample_range = {-20,20,-20,20,-10,10,-10,10};
    QuasiCrystal AmmBee = {basis, Shape(Octagon(s)),
                           "Ammann Beenker", sample_range};
    AmmBee.patterns_.emplace_back(
            std::make_unique<Window>(Shape(Polygon(Pattern1(s)))));
    return AmmBee;
}
}
