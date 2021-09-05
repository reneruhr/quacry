#pragma once
#include "kipod.h"
#include "quasicrystal.h"

namespace kipod::QuasiCrystals{

class QuasiCrystalsScene :  public kipod::Listener, public kipod::Controls, public kipod::RenderScene{
        friend class QuasiCrystalsSidebar;
        friend class QuasiCrystalsConsole;
        friend class QuasiCrystalsModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        using QuasiCrystalContainer = std::vector<std::unique_ptr<QuasiCrystal>>;
        using Projection = kipod::RenderCamera;
        using ProjectionContainer = std::vector<std::unique_ptr<RenderCamera> > ;

        QuasiCrystal* active_quacry_ = nullptr;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupLayout(QuasiCrystal* quacry);
        void SetupShaders();
        void SetupUniforms(Projection *projection, QuasiCrystal *quacry);

        void SetUniformInternal(Projection *projection, QuasiCrystal *quacry);
        void SetUniformPhysical(Projection *projection, QuasiCrystal *quacry);
        void SetUniformPhysicalWithEdges(Projection *projection, QuasiCrystal *quacry);
        void SetUniformPhysicalBox(Projection *projection, QuasiCrystal *quacry);
        void SetUniformWindow(Window *window);

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

        void AddQuasiCrystal(QuasiCrystal&&);
        QuasiCrystal* ActiveQuasiCrystal();
        void ActiveQuasiCrystal(QuasiCrystal*);
        bool HasQuasiCrystal();

        Projection* ActiveProjection();
        void ActiveProjection(Projection*);
};
}

