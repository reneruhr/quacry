#pragma once
#include "../quasicrystal.h"
#include <Eigen/Dense>
namespace quacry{
    using Mat5f = Eigen::Matrix<float,5,5>;
    using Vec5f = Eigen::Matrix<float,5,1>;
    Quasicrystal23 Penrose();

    auto PenroseRotation() -> Mat5f;
    auto PenroseInternalPolytope(Mat5f g, const Vec5f& gamma) -> std::pair<std::vector<Vec3>,std::vector<unsigned int>>;
}

