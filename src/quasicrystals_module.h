#pragma once
#include "kipod.h"

namespace quacry{

class QuasiCrystalsModule : public kipod::Module
{
public:
    QuasiCrystalsModule(std::string name, int width = 1024, int height = 768);
};

}
