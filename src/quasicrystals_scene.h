#pragma once
#include "kipod.h"
#include "quasicrystal.h"
#include "../kipod/src/modules/meshmodels/meshmodel_module.h"


namespace quacry{
using namespace kipod;
using namespace kipod::MeshModels;
class QuasiCrystalsScene :  public kipod::Listener, public kipod::Controls, public kipod::RenderScene{
        friend class QuasiCrystalsSidebar;
        friend class QuasiCrystalsConsole;
        friend class QuasiCrystalsModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        std::unique_ptr<MeshModelModule> meshmodel_module_; 


        using QuasiCrystalContainer = std::vector<std::unique_ptr<Quasicrystal>>;
        using Projection = kipod::RenderCamera;
        using ProjectionContainer = std::vector<std::unique_ptr<RenderCamera> > ;

        Quasicrystal* active_quacry_ = nullptr;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupMeshModelModule();

        void SetupOptions();

        void SetupShaders();
        void SetupUniforms(Projection *projection, Quasicrystal22 *quacry);

        void SetUniformInternal(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysical(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysicalWithEdges(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPhysicalBox(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformWindow(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformPattern(Projection *projection, Quasicrystal22 *quacry);
        void SetUniformQuasicrystal23(Projection *projection, Quasicrystal23 *quacry);
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
        void AddQuasiCrystal(Quasicrystal23&&);
        auto ActiveQuasiCrystal() -> Quasicrystal*;
        void ActiveQuasiCrystal(Quasicrystal*);
        bool HasQuasiCrystal();

        Projection* ActiveProjection();
        void ActiveProjection(Projection*);

        auto GetMeshModelScene() -> MeshModelScene*;
        auto GetMeshModelModule() -> MeshModelModule*;

};
}

