#pragma once
#include "kipod.h"
namespace quacry{

class QuasiCrystalsSidebar: public kipod::Sidebar{
friend class QuasiCrystalsModule;
public:
protected:
    explicit QuasiCrystalsSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    void SideBarContent() override;

    void AddQuasiCrystal();
    void QuasiCrystalsList();
    void LatticeControl();
    void WindowControl();
    void ViewOptions();
};
}
