#pragma once
#include "../quasicrystal.h"
#include <Eigen/Dense>

namespace quacry{
auto MakeGenericQuasi23(float scale = 1.0f) -> Quasicrystal23;
}
