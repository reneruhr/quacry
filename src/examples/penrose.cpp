#include "penrose.h"
#include <Eigen/Dense>
#include "../../external_libs/quickhull/QuickHull.hpp"
namespace quacry{
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using Vec5f = Eigen::Matrix<float,5,1>;
using Mat5f = Eigen::Matrix<float,5,5>;
using namespace kipod::MeshModels;

auto PenroseRotation() -> Mat5f 
{
    Mat5f g;
    float c = std::sqrt(2./5);
    float t = 2./5 * 3.1415926535;
    float u = 1./std::sqrt(2.);
    auto si = [=](int i){ return sin(i*t); };
    auto co = [=](int i){ return cos(i*t); };
    for(int j = 0; j<5; ++j)
        g.col(j) = c*Vec5f(co(j), si(j), co(2*j), si(2*j), u);

    return g;
}

auto PenroseInternalPolytope(Mat5f g, Vec5f gamma) -> std::pair<std::vector<Vec3>,std::vector<unsigned int>>
{
    LOG_INFO("Creating Internal Penrose Polytope from data g={} and gamma={}", g,gamma);

    using namespace quickhull;
    QuickHull<float> qh;
    auto vertices = std::vector<Vec3>();
    auto g_penrose = PenroseRotation();
    g = g*g_penrose;
    auto unit_cube = std::vector<Vec5f>();
    auto s = [](int i, int j){ return (i & (1 << j)) == 0 ? -0.5 : 0.5; };
    LOG_INFO("Creating 5-dim Unitcube:");
    for(int i = 0; i<32; ++i){
       Vec5f v = { s(i,0), s(i,1),s(i,2),s(i,3),s(i,4)}; 
       unit_cube.push_back(v); 
       LOG_INFO("{}th vector = {}", i, v.transpose());
    }
    for(auto& v : unit_cube) v = g*(v + gamma);
    for(const auto& v : unit_cube) vertices.push_back({v[2],v[3],v[4]});
    auto hull = qh.getConvexHull((float*)vertices.data(), vertices.size(), true, false);
    auto hull_vertices = std::vector<glm::vec3>();
    auto indices = std::vector<unsigned int>();
    for(auto& v : hull.getVertexBuffer()) hull_vertices.push_back({v.x,v.y,v.z});
    for(auto& v : hull.getIndexBuffer()) indices.push_back(v);
    return std::make_pair(hull_vertices, indices);
}

auto Penrose() -> Quasicrystal23
{
    auto [v,i] = PenroseInternalPolytope(Mat5f::Identity(), Vec5f::Constant(0.25f));
    auto penrose = Quasicrystal23("Penrose", PenroseRotation(), MeshModel(v,i));

    return penrose;
}
}
