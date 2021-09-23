#pragma once
#include "kipod.h"
#include "quasicrystal.h"

namespace quacry{
using namespace kipod;

class QuasiCrystalsScene :  public kipod::Listener, public kipod::Controls, public kipod::RenderScene{
        friend class QuasiCrystalsSidebar;
        friend class QuasiCrystalsConsole;
        friend class QuasiCrystalsModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        using QuasiCrystalContainer = std::vector<std::unique_ptr<Quasicrystal22>>;
        using Projection = kipod::RenderCamera;
        using ProjectionContainer = std::vector<std::unique_ptr<RenderCamera> > ;

        Quasicrystal22* active_quacry_ = nullptr;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupLayout(Quasicrystal22* quacry);
        void SetupShaders();
        void SetupUniforms(Projection *projection, Quasicrystal22 *quacry);

        void SetUniformInternal(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysical(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysicalWithEdges(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysicalBox(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformWindow(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPattern(Projection *projection, Quasicrystal22 *quacry);

public:
        QuasiCrystalsScene(int width, int height);
        QuasiCrystalsScene(QuasiCrystalsScene&) = delete;
        QuasiCrystalsScene& operator=(QuasiCrystalsScene&) = delete;
        QuasiCrystalsScene(QuasiCrystalsScene&&) = default;
        QuasiCrystalsScene& operator=(QuasiCrystalsScene&&) = delete;
        ~QuasiCrystalsScene() = default;

protected:
        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;

        QuasiCrystalContainer quacries_;
        ProjectionContainer& projections_ = RenderScene::cameras_;
        std::unique_ptr<Projection> internal_projection_;

        void AddQuasiCrystal(Quasicrystal22&&);
        Quasicrystal22* ActiveQuasiCrystal();
        void ActiveQuasiCrystal(Quasicrystal22*);
        bool HasQuasiCrystal();

        Projection* ActiveProjection();
        void ActiveProjection(Projection*);
};
}

