#pragma once
#include "../../kipod.h"

namespace kipod::QuasiCrystals{
class QuasiCrystalsConsole :
        public kipod::Console{
        friend class QuasiCrystalsModule;
public:
    QuasiCrystalsConsole(std::shared_ptr<kipod::RenderScene> scene) : kipod::Console(scene) {}

    virtual void Draw() override {
        DrawModuleConsole();
    }
    virtual void ConsoleContent() override;
};

}
