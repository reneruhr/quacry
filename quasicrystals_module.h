#pragma once
#include "../../kipod.h"

namespace kipod::QuasiCrystals{

class QuasiCrystalsModule : public kipod::Module
{
public:
    QuasiCrystalsModule(std::string name, int width = 1024, int height = 768);
};

}
