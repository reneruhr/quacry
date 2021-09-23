#include "ammannbeenker.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>
namespace quacry{
using Vec2 = glm::vec2;
using namespace kipod::Shapes;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

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

auto Pattern3(float s) -> std::vector<Vec2>
{
    auto o = Octagon(s);

    auto i = FindIntersection(o.GetVertex(6), o.GetVertex(1), o.GetVertex(7), o.GetVertex(4));
    auto j = FindIntersection(o.GetVertex(0), o.GetVertex(5), o.GetVertex(6), o.GetVertex(1));
    auto k = FindIntersection(o.GetVertex(0), o.GetVertex(5), o.GetVertex(7), o.GetVertex(4));
    auto toflip = i-j; toflip.x *= -1;
    auto jk = j+toflip;
    return {i,j,jk,k};
}

auto Pattern4(float s) -> std::vector<Vec2>
{
    auto o = Octagon(s);

    auto i = FindIntersection(o.GetVertex(6), o.GetVertex(1), o.GetVertex(7), o.GetVertex(4));
    auto j = FindIntersection(o.GetVertex(0), o.GetVertex(5), o.GetVertex(6), o.GetVertex(1));
    auto k = FindIntersection(o.GetVertex(0), o.GetVertex(5), o.GetVertex(7), o.GetVertex(4));
    auto toflip = i-j; toflip.x *= -1;
    auto jk = j+toflip;

    
    auto c = FindIntersection(j,j+Vec2(-1,0), o.GetVertex(7)+Vec2(-s,0), o.GetVertex(6)+Vec2(-s,0));
    auto d = FindIntersection(jk,k, o.GetVertex(7)+Vec2(-s,0), o.GetVertex(6)+Vec2(-s,0));
    return {jk, j, c, d};
}

auto Pattern5(float s) -> std::vector<Vec2>
{
    auto prev = Pattern4(s);
    auto a = prev[0];
    auto b = prev[3];
    auto toflip = b-a; toflip.y *= -1;
    auto c = a + toflip;
    return {a,b,c};
}

auto Pattern6(float s) -> std::vector<Vec2>
{
    double lambda = -1.+std::sqrt(2.);
    auto o = Octagon(s);
    auto vs = o.transformed_vertices_;
    for(auto& v : vs) v*= lambda*lambda;
    return vs;
}

auto Rotate8(const Vec2& in) -> std::vector<Vec2>
{
    std::vector<Vec2> out;
    auto theta = [](int i){ return 	3.1415926535897*2./i; };
    for(int i = 0; i<8; ++i)
        out.emplace_back(Mat2(cos(theta(i)),sin(theta(i)),-sin(theta(i)),cos(theta(i)))*in);
    return out;
}

auto AmmannBeenker() -> Quasicrystal22 
{
    const float s = std::sqrt(2);

    Basis4 basis = {1,0,s,0,
                   1,2,0,s,
                   1,0,-s,0,
                   1,2,0,-s}; basis = glm::transpose(basis);

    SampleSize sample_range = {-20,20,-20,20,-10,10,-10,10};
    Quasicrystal22 AmmBee = {basis, Shape(Octagon(s)),
                           "Ammann Beenker", sample_range};
    auto vs = Pattern1(s); auto polygon1 = Polygon{vs,AvgPoint(vs)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon1)));
    
    auto vs2 = Pattern2(s); auto polygon2 = Polygon{vs2,AvgPoint(vs2)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon2)));

    auto vs3 = Pattern3(s); auto polygon3 = Polygon{vs3,AvgPoint(vs3)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon3)));

    auto vs4 = Pattern4(s); auto polygon4 = Polygon{vs4,AvgPoint(vs4)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon4)));
    
    auto vs5 = Pattern5(s); auto polygon5 = Polygon{vs5,AvgPoint(vs5)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon5)));

    auto vs6 = Pattern6(s); auto polygon6 = Polygon{vs6,AvgPoint(vs6)};
    AmmBee.patterns_.emplace_back( std::make_unique<Window2>(Shape(polygon6)));

    for(auto& p : AmmBee.patterns_)
        p->Init();
    return AmmBee;
}
}
