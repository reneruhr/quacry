#pragma once
#include "kipod.h"
namespace kipod::QuasiCrystals{

class QuasiCrystalsSidebar: public kipod::Sidebar{
friend class QuasiCrystalsModule;
public:
protected:
    QuasiCrystalsSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

    void AddQuasiCrystal();
    void QuasiCrystalsList();
    void LatticeControl();
    void WindowControl();
    void ViewOptions();
};
}
