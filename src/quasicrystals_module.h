#pragma once
#include "kipod.h"
#include "quasicrystals_scene.h"

namespace quacry{

class QuasiCrystalsModule : public kipod::Module
{
public:
    QuasiCrystalsModule(std::string name, int width = 1024, int height = 768);
    auto GetScene() -> QuasiCrystalsScene*;
};

class QuasiCrystalsModuleMenu : public kipod::MenuModule
{
    friend class QuasiCrystalsModule;
    QuasiCrystalsScene* scene_;
protected:
    void Draw() override;
public:
    QuasiCrystalsModuleMenu(QuasiCrystalsScene* scene)
        : scene_(scene){}
};
}
