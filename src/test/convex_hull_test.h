#pragma once
#include "kipod.h"
#include "../quasicrystals_module.h"
#include "../../external_libs/quickhull/QuickHull.hpp"

void ConvexHullTest(kipod::Application& kipod)
{
    using namespace kipod;
    using namespace kipod::MeshModels;
    auto [width, height] = kipod.WindowSize();
    kipod.Add(kipod::MeshModels::MeshModelModule("Convex Hull Test", width, height));
    using namespace quickhull;
    QuickHull<float> qh;
    auto hull_module = kipod.GetModule("Convex Hull Test");
    auto scene = static_cast<MeshModelModule*>(hull_module)->GetScene();
    auto bob = scene->AddModel("kipod/resources/meshmodels/bob.obj");
    auto vertices = bob->Vertices();
    auto hull = qh.getConvexHull((float*)vertices->data(), vertices->size(), true, false);
    auto hull_vertices = std::vector<glm::vec3>();
    auto indices = std::vector<unsigned int>();
    for(auto& v : hull.getVertexBuffer()) hull_vertices.push_back({v.x,v.y,v.z});
    for(auto& v : hull.getIndexBuffer()) indices.push_back(v);
    auto bobs_hull = MeshModel(hull_vertices, indices);
    scene->AddModel(bobs_hull);
}
