// clang-format off
#include <Pepper.hpp>
#include <PepperPCH.hpp>
// clang-format on

#include <imgui.h>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void OnImGuiRender() override
  {
    ImGui::Begin("Pepper");
    ImGui::Text("Hello, Pepper!");
    ImGui::End();
  }

  void OnUpdate() override
  {
    if (Pepper::Input::IsKeyPressed(PP_KEY_TAB))
      PP_INFO("Tab pressed");
  }

  void OnEvent(Pepper::Event&) override {}
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp() { PushLayer(new ExampleLayer{}); }
  ~ClientApp() {}
};

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }