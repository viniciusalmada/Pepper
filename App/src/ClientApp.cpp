#include <Pepper.hpp>
#include <PepperPCH.hpp>

class ExampleLayer : public Pepper::Layer
{
public:
  ExampleLayer() : Layer("Example") {}

  void OnUpdate() override { PP_INFO("ExampleLayer::Update"); }

  void OnEvent(Pepper::Event& event) override { PP_TRACE("{0}", event); }
};

class ClientApp : public Pepper::Application
{
public:
  ClientApp() { PushLayer(new ExampleLayer{}); }
  ~ClientApp() {}
};

Pepper::Application* Pepper::CreateApplication() { return new ClientApp(); }