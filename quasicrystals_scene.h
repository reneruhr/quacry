#pragma once
#include "../../kipod.h"
#include "quasicrystal.h"


namespace kipod::QuasiCrystals{


class QuasiCrystalsScene :  public kipod::Listener,
                     public kipod::Controls,
                     public kipod::RenderScene{
                     friend class QuasiCrystalsSidebar;
                     friend class QuasiCrystalsModule;


        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;


        using QuasiCrystalContainer = std::vector<std::unique_ptr<QuasiCrystal>>;
        using Camera = kipod::RenderCamera;

        QuasiCrystal* active_quacry_ = nullptr;



        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupLayout(QuasiCrystal* quacry);
        void SetupShaders();
        void SetupUniforms(Camera* camera, QuasiCrystal *quacry);

        void SetUniformInternal(Camera *camera, QuasiCrystal *quacry);
        void SetUniformPhysical(Camera *camera, QuasiCrystal *quacry);
        void SetUniformPhysicalBox(Camera *camera, QuasiCrystal *quacry);


public:
        QuasiCrystalsScene(int width, int height);
        QuasiCrystalsScene(QuasiCrystalsScene&) = default;
        QuasiCrystalsScene& operator=(QuasiCrystalsScene&) = default;
        QuasiCrystalsScene(QuasiCrystalsScene&&) = default;
        QuasiCrystalsScene& operator=(QuasiCrystalsScene&&) = default;
        //~QuasiCrystalsScene() = default;

protected:
        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;


        QuasiCrystalContainer quacries_;

        void AddQuasiCrystal(QuasiCrystal&&);
        QuasiCrystal* ActiveQuasiCrystal();
        void ActiveQuasiCrystal(QuasiCrystal*);
        bool HasQuasiCrystal();

};

}

