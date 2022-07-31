#include <Pepper.hpp>
#include <PepperPCH.hpp>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void OnUpdate() override
  {
    if (Pepper::Input::IsKeyPressed(PP_KEY_TAB))
      PP_INFO("Tab pressed");
  }

  void OnEvent(Pepper::Event& event) override { /* PP_TRACE("{0}", event); */ }
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp()
  {
    PushLayer(new ExampleLayer{});
    PushLayer(new Pepper::ImGuiLayer{});
  }
  ~ClientApp() {}
};

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }