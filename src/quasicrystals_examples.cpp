#include "quasicrystals_examples.h"
#include <Eigen/Geometry>
namespace quacry{
using Vec2 = glm::vec2;
using namespace kipod::Shapes;

auto GetOrthogonalLine(const Vec2&a, const Vec2& b) -> Vec2
{
    Vec2 dir = b-a;
    return {-dir.y, dir.x};
}

auto FindIntersection(const Vec2& a,const Vec2& b, const Vec2& x, const Vec2& y) -> Vec2
{
    using Line2 = Eigen::Hyperplane<float,2>;
    auto ToEigen = [](Vec2 x){ return Eigen::Vector2f{x.x,x.y}; };
    Line2 A = Line2::Through(ToEigen(a), ToEigen(b));
    Line2 B = Line2::Through(ToEigen(x), ToEigen(y));
    Eigen::Vector2f p = A.intersection(B);
    LOG_INFO("Intersection found: {}", p);
    return {p[0],p[1]};
}

auto AvgPoint(const std::vector<Vec2>& vs) -> Vec2
{
    return std::accumulate(begin(vs),end(vs),Vec2(0))/float(size(vs));
}

auto Pattern1(float s) ->std::vector<Vec2>
{   //      ____a   0
    //          \
    //           \b   7
    //         i/ |
    //          \ |c  6
    //           /
    //     _____/d   5
    auto o = Octagon(s);
    auto a = o.GetVertex(0);
    auto b = o.GetVertex(7);
    auto c = o.GetVertex(6);
    auto d = o.GetVertex(5);
    auto i = FindIntersection(b, 
                              b+GetOrthogonalLine(b, a),
                              c,
                              c+GetOrthogonalLine(d,c)); 
    return {c,b,i};
}

auto Pattern2(float s) ->std::vector<Vec2>
{
    auto o = Octagon(s);
    auto a = o.GetVertex(0);
    auto b = o.GetVertex(7);
    auto c = o.GetVertex(6);
    auto d = o.GetVertex(5);
    auto e = o.GetVertex(4);
    auto f = o.GetVertex(3);
    auto g = o.GetVertex(2);
    auto h = o.GetVertex(1);
    auto i = FindIntersection(a,d,b,g);
    auto j = FindIntersection(a,d,c,h);
    auto k = FindIntersection(c,h,b,e);
    return {b,i,j,k};
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
    auto vs = Pattern1(s); auto polygon1 = Polygon{vs,AvgPoint(vs)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window>(Shape(polygon1)));
    
    auto vs2 = Pattern2(s); auto polygon2 = Polygon{vs2,AvgPoint(vs2)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window>(Shape(polygon2)));


    for(auto& p : AmmBee.patterns_)
        p->Init();
    return AmmBee;

}
}
